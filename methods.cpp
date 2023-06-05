#include "headers/ConfigFile.hpp"
#include <sys/types.h>
#include <sys/wait.h>

string  generateAutoIndex(string path)
{
    stringstream listing;
    listing << "<html><head><title>Index of " << path << " </title></head>";
    listing << "<body><h1>Index of " << path << "</h1>\n";
    listing << "<ul>";

    DIR* dir = opendir(path.c_str());
    if (dir)
    {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL)
        {
            string name = entry->d_name;
            listing << "<li><a href=\"" << name << "\">" << name << "</a></li>";
        }
        closedir(dir);
    }

    listing << "</ul></body></html>";
    return listing.str();
}

bool isDirectory(const string path)
{
    DIR* dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return true;
    }
    return false;
}

bool isCGI(string path, string ext)
{
    if (!ext.size())
        return (false);
    if (!path.compare(path.size() - ext.size(), path.size(), ext))
        return (true);
    else
        return (false);
}

vector<string> split(string request, string lims) {
    vector<string> end;
    size_t x = request.find(lims);
    if (x == string::npos)
        return (end.push_back(request), end);
    end.push_back(request.substr(0, x));
    end.push_back(request.substr(x + lims.length(), request.length() - x));
    return end;
}

int    CGI_response(vector<string> cgi_out, t_client& client)
{
    string Content_Type;

    stringstream ss2( cgi_out[0]);
    if (getline(ss2, Content_Type, ' '))
    {
        if (Content_Type.compare("Content-Type:"))
            throw runtime_error("unwanted value1");// 7ta nlo7o chi error code hna
        getline(ss2, Content_Type, ';');
    }

    return (GenerateResponse(cgi_out[1], Content_Type, 200, client, ""), 0);
}

int    CGI_handler(t_client& client, Server server, string path_to_serve, int loc_pos)
{
    int     pipes[2];
    int     status = 0;
    pid_t   pid;
    string  body;
    char    c;

    
    if (pipe(pipes) == -1)
        throw runtime_error("pipe failed");
    
    pid = fork();
    if (pid == -1)
        throw runtime_error("fork failed");
    else if (pid == 0)
    {
        if (dup2(pipes[1], STDOUT_FILENO) == -1 || close(pipes[1]) == -1 || close(pipes[0]) == -1)
	    	throw runtime_error("dup2() or close() failed");

        if (client.cookie.size())
            setenv("HTTP_COOKIE", client.cookie["sessionID"].c_str(), 1);
        setenv("REQUEST_METHOD", "GET", 1);
        setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
        setenv("PATH_INFO", client.request["path"].substr(server.getLocation(loc_pos)->getPath().size(), client.request["path"].find("?")).c_str(), 1);
        setenv("SCRIPT_FILENAME", path_to_serve.append(client.request["path"].substr(server.getLocation(loc_pos)->getPath().size())).c_str(), 1);
        setenv("REDIRECT_STATUS", "", 1);
        setenv("SERVER_NAME", server.getValue("host").c_str(), 1);
        setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);

        extern char** environ;
        char** env = environ;

        const string& cgiPath = server.getLocation(loc_pos)->get_cgi_path();
        const char* arg1 = cgiPath.c_str();

        char* args[] = {(char*)arg1, (char*)path_to_serve.c_str(), NULL};

        if (execve(cgiPath.c_str(), args, env) == -1)
            cout << strerror(errno) << endl;
        cout << "mok" << endl;
        exit(1);
    }
    else
    {
        if (close(pipes[1]) == -1)
            throw runtime_error("dup2() or close() failed");
        waitpid(pid, &status, 0);
        if (status != 0)
            throw runtime_error("toooooooooooo");

        while (read(pipes[0], &c, 1) > 0)
            body.push_back(c);
        close(pipes[0]);

        vector<string> cgi_out = split(body, "\r\n\r\n");
        if (cgi_out.size() == 1)
        {
            // that mean im only have the body and am gonna assume the contect-type contect-length
            return (GenerateResponse(cgi_out[cgi_out.size() - 1], "text/plain", 200, client, ""), 0);
        }
        else if (cgi_out.size() == 2)
        {
            CGI_response(cgi_out, client);
        }
        else
        {
            cerr << "error in cgi script output" << endl;
        }
        //     throw runtime_error("no heder or body in cgi_out error 502");
        body = cgi_out[cgi_out.size() - 1];
        
    }
    return (0);
}

int    GetMethod(t_client& client, Server server)
{
    string req_path;
    string path_to_serve;
    string test_file;
    int loc_pos;
    int fd;

    req_path = client.request["path"].substr(0, client.request["path"].find("?"));
    loc_pos = getRightLocation(req_path, server);
    path_to_serve = getRightRoot(server, loc_pos);

    // is cgi request
    if (isCGI(req_path, server.getLocation(loc_pos)->get_cgi_ext()))
        return (CGI_handler(client, server, path_to_serve, loc_pos));

    // if we dont have a spesific file in that location
    if (req_path.substr(server.getLocation(loc_pos)->getPath().size()).size() == 0 || req_path.at(req_path.size() - 1) == '/')
    {
    //////////////////------ get right index file ------////////////////////
        if (req_path.compare("/"))
        {
            if (isDirectory(path_to_serve.append(req_path.substr(server.getLocation(loc_pos)->getPath().size()))))
            {
                if (req_path.at(req_path.size() - 1) != '/')
                    return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), "text/html", 301, client, ""), 1);
                else if (!server.getValue("autoindex").compare("ON") || !server.getLocation(loc_pos)->getAutoIndex().compare("ON"))
                    return (GenerateResponse(generateAutoIndex(path_to_serve), ".html", 200, client, ""), 0);
            }
        }
        test_file = path_to_serve;
        for (size_t i = 0; i < server.getLocation(loc_pos)->getIndexSize(); i++)
        {
            fd = open(test_file.append("/").append(server.getLocation(loc_pos)->getIndex(i)).c_str(), O_RDONLY);
            if (fd != -1)
                    return (GenerateResponse(getRightContent(fd), getContentType(server.getLocation(loc_pos)->getIndex(i)), 200, client, ""), 0);
            test_file = path_to_serve;
        }
        return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), "text/html", 404, client, ""), 1);
    }
    else
    {
        ////////////////------  check file   ------//////////////////

        // GET RIGHT ROOT
        if (!server.getLocation(loc_pos)->getPath().compare("/") && !req_path.compare("/"))
            path_to_serve.append(req_path.substr(1));
        else if (server.getLocation(loc_pos)->getRoot().size())
            path_to_serve = server.getLocation(loc_pos)->getRoot().append(req_path.substr(server.getLocation(loc_pos)->getPath().size()));
        else if (server.getValue("root").size())
            path_to_serve = server.getValue("root").append(req_path);
        else
            return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), "text/html", 405, client, ""), 1); //

        if (isDirectory(path_to_serve))
        {
            if (req_path.at(req_path.size() - 1) != '/')
                return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), "text/html", 301, client, ""), 1);
            return (GenerateResponse(generateAutoIndex(path_to_serve), ".html", 200, client, ""), 0);
        }
        fd = open(path_to_serve.c_str(), O_RDONLY);
        if (fd != -1)
            return (GenerateResponse(getRightContent(fd), getContentType(req_path), 200, client, ""), 0);
        if (isDirectory(path_to_serve))
        {
            if (!server.getValue("autoindex").compare("ON") || !server.getLocation(loc_pos)->getAutoIndex().compare("ON"))
                return (GenerateResponse(generateAutoIndex(path_to_serve), ".html", 200, client, ""), 0);
        }
        else
            return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), "text/html", 404, client, ""), 1);
    }
    return (1);
}

int    CGI_handler_post(t_client& client, Server server, string path_to_serve, int loc_pos)
{
    int     pipes[2];
    int     status = 0;
    pid_t   pid;
    string  body;
    char    c;

    cout << client.body << endl;
    if (pipe(pipes) == -1)
        throw runtime_error("pipe failed");
    
    pid = fork();
    if (pid == -1)
        throw runtime_error("fork failed");
    else if (pid == 0)
    {
        if (dup2(pipes[1], STDOUT_FILENO) == -1 || close(pipes[1]) == -1 || close(pipes[0]) == -1)
	    	throw runtime_error("dup2() or close() failed");

        if (client.cookie.size())
            setenv("HTTP_COOKIE", client.cookie["sessionID"].c_str(), 1);
        if (!client.request["method"].compare("GET"))
            setenv("REQUEST_METHOD", "GET", 1);
        else
            setenv("REQUEST_METHOD", "POST", 1);
        setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
        setenv("PATH_INFO", client.request["path"].substr(server.getLocation(loc_pos)->getPath().size(), client.request["path"].find("?")).c_str(), 1);
        setenv("SCRIPT_FILENAME", path_to_serve.append(client.request["path"].substr(server.getLocation(loc_pos)->getPath().size())).c_str(), 1);
        setenv("REDIRECT_STATUS", "", 1);
        setenv("SERVER_NAME", server.getValue("host").c_str(), 1);
        setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
        setenv("CONTENT_TYPE", client.request["media-type"].c_str(), 1);
        setenv("CONTENT_LENGTH", client.request["length"].c_str(), 1);

        extern char** environ;
        char** env = environ;

        const string& cgiPath = server.getLocation(loc_pos)->get_cgi_path();
        const char* arg1 = cgiPath.c_str();

        char* args[] = {(char*)arg1, (char*)path_to_serve.c_str(), NULL};
        if (!client.request["method"].compare("POST"))
        {
            ofstream tmpfile(path_to_serve);
            if (!tmpfile.is_open())
                return (cerr << "faild to open tmpfile" << endl, exit(1), 1);
            tmpfile << client.body;
            tmpfile.close();
        }
        if (execve(cgiPath.c_str(), args, env) == -1)
            cout << strerror(errno) << endl;
        cout << "mok" << endl;
        exit(1);
    }
    else
    {
        if (close(pipes[1]) == -1)
            throw runtime_error("dup2() or close() failed");
        waitpid(pid, &status, 0);
        if (status != 0)
            throw runtime_error("toooooooooooo");

        while (read(pipes[0], &c, 1) > 0)
            body.push_back(c);
        close(pipes[0]);

        vector<string> cgi_out = split(body, "\r\n\r\n");
        if (cgi_out.size() == 1)
        {
            // that mean im only have the body and am gonna assume the contect-type contect-length
            return (GenerateResponse(cgi_out[cgi_out.size() - 1], "text/plain", 200, client, ""), 0);
        }
        else if (cgi_out.size() == 2)
        {
            CGI_response(cgi_out, client);
        }
        else
        {
            cerr << "error in cgi script output" << endl;
        }
        //     throw runtime_error("no heder or body in cgi_out error 502");
        body = cgi_out[cgi_out.size() - 1];
        
    }
    return (0);
}

void    PostMethod(t_client& client, Server& server)
{
    string filename;
    int L = 0;
    std::ofstream postfile;

    L = getRightLocation(client.request["path"], server);
    filename = client.request["path"].substr(client.request["path"].find_last_of('/', string::npos));
    cout << server.getLocation(L)->getPath() << endl;

    if (isCGI(client.request["path"].substr(0, client.request["path"].find("?")), server.getLocation(L)->get_cgi_ext()))
    {
        CGI_handler_post(client, server, getRightRoot(server, L), L);
        return ;
    }

    if(!filename.compare(server.getLocation(L)->getPath()))
        filename = filename.substr(server.getLocation(L)->getPath().size());
    if (filename.size() == 0 || !filename.compare("/"))
        filename = '/'+ client.request["filename"];

    cout << getRightRoot(server, L) << endl;
    cout << server.getLocation(L)->get_upload_dir() << endl;
    cout << filename << endl;
    filename = getRightRoot(server, L) + server.getLocation(L)->get_upload_dir() + filename;
    postfile.open(filename,std::ios::binary);
    if (postfile)
    {
        if(client.request["boundary"].size())
            postfile.write(client.body.c_str(), client.body.find(client.request["boundary"]));
        else
            postfile.write(client.body.c_str(), client.body.size());
        GenerateResponse("", "", 201, client, "");
    }
    else
        GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), ".text/html", 500, client, "");
}

void    DeleteMethod(t_client& client, Server server)
{
    string filename;
    std::ofstream postfile;
    struct dirent *pDirent;
    DIR *dir;
    int L = 0;
    //check the path of the Request and if post is allowed
    L = getRightLocation(client.request["path"], server);
    filename = getRightRoot(server, L) + '/' + client.request["path"].substr(server.getLocation(L)->getPath().size());
    dir = opendir(filename.c_str());
    if(dir != NULL)
    {
        while((pDirent = readdir(dir)) != NULL)
            remove((filename +'/'+ pDirent->d_name).c_str());
    }
    remove(filename.c_str());
    GenerateResponse("", "", 204, client, "");
}