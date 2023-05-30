#include "headers/ConfigFile.hpp"

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

// bool isCGI(string path, string ext)
// {
//     if (!path.compare(path.size() - ext.size(), path.size(), ext))
//         return (true);
//     else
//         return (false);
// }

// int    CGI_handler(t_client& client, Server server, int loc_pos)
// {
//     int     pipes[2];
//     int     status;
//     pid_t   pid, wait;
    
//     if (pipe(pipes) == -1)
//         throw std::runtime_error("pipe failed");
    
//     pid = fork();
//     if (pid == -1)
//         throw std::runtime_error("fork failed");
//     else if (pid)
//     {
//         wait = waitpid(pid, &status, 0);
//         if (wait == -1)
//             throw std::runtime_error("wait failed");
//         if (WIFEXITED(status))
//             return WEXITSTATUS(status);
//         else if (WIFSIGNALED(status))
// 		    return WTERMSIG(status);
//         else
//             return (1);
//     }
//     else
//     {
//         if (dup2(pipes[0], STDIN_FILENO) == -1 || close(pipes[1]) == -1 || close(pipes[0]) == -1)
// 	    		throw std::runtime_error("dup2() of close() failed");

//         setenv("REQUEST_METHOD", "GET", 1);
//         if (server.getLocation(loc_pos).get)
//         setenv("PATH_INFO", server.getValue("root").c_str(), 1);
//         extern char** environ;
//         char** env = environ;
//         // need args for execve and so one
//     }
// }

int    GetMethod(t_client& client, Server server)
{
    string req_path;
    string path_to_serve;
    string test_file;
    int loc_pos;
    int fd;

    req_path = client.request["path"].substr(0, req_path.find("?"));
    loc_pos = getRightLocation(req_path, server);
    path_to_serve = getRightRoot(server, loc_pos, client);

    // // is cgi request
    // if (isCGI(req_path, server.getLocation(loc_pos)->get_cgi_ext()))
    //     CGI_handler(client, server, loc_pos);

    // if we dont have a spesific file in that location
    if (req_path.substr(server.getLocation(loc_pos)->getPath().size()).size() == 0 || req_path.at(req_path.size() - 1) == '/')
    {
    //////////////////------ get right index file ------////////////////////
        if (req_path.compare("/"))
        {
            if (isDirectory(path_to_serve.append(req_path)))
            {
                if (req_path.at(req_path.size() - 1) != '/')
                    return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), ".html", 301, client), 1);
                else if (!server.getValue("autoindex").compare("ON") || !server.getLocation(loc_pos)->getAutoIndex().compare("ON"))
                    return (GenerateResponse(generateAutoIndex(path_to_serve), ".html", 200, client), 0);
            }
        }
        test_file = path_to_serve;
        for (size_t i = 0; i < server.getLocation(loc_pos)->getIndexSize(); i++)
        {
            fd = open(test_file.append("/").append(server.getLocation(loc_pos)->getIndex(i)).c_str(), O_RDONLY);
            if (fd != -1)
                    return (GenerateResponse(getRightContent(fd), getContentType(server.getLocation(loc_pos)->getIndex(i)), 200, client), 0);
            test_file = path_to_serve;
        }
        return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), ".html", 404, client), 1);
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
            return (GenerateResponse("", "", 405, client), 1); //

        if (isDirectory(path_to_serve))
        {
            if (req_path.at(req_path.size() - 1) != '/')
                return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), ".html", 301, client), 1);
            return (GenerateResponse(generateAutoIndex(path_to_serve), ".html", 200, client), 0);
        }
        fd = open(path_to_serve.c_str(), O_RDONLY);
        if (fd != -1)
            return (GenerateResponse(getRightContent(fd), getContentType(req_path), 200, client), 0);
        if (isDirectory(path_to_serve))
        {
            if (!server.getValue("autoindex").compare("ON") || !server.getLocation(loc_pos)->getAutoIndex().compare("ON"))
                return (GenerateResponse(generateAutoIndex(path_to_serve), ".html", 200, client), 0);
        }
        else
            return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), ".html", 404, client), 1);
    }
    return (1);
}

void    PostMethod(t_client& client, Server& server)
{
    string filename;
    int L = 0;
    std::ofstream postfile;

    L = getRightLocation(client.request["path"], server);
    filename = client.request["path"].substr(client.request["path"].find_last_of('/', string::npos));

    if (filename.size() == 0)
        filename = generateRandomString(10) + "." +client.request["Content-Type"];

    filename = getRightRoot(server, L, client) + server.getLocation(L)->get_upload_dir() + filename;

    postfile.open(filename,std::ios::binary);
    if (postfile)
    {
        cout << client.body.find("\r\n") << endl;
        postfile.write(client.body.c_str(), client.body.find(client.request["boundary"]));
        GenerateResponse("", "", 201, client);
    }
    else
        GenerateResponse("", "", 500, client);
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
    /*if(!count(server.getLocation(L)->getAllowedMethod().begin(),server.getLocation(L)->getAllowedMethod().end(),"DELETE"))
    {
            //error METHOD NOT ALLOWED
    }*/
    filename = getRightRoot(server, L, client) + client.request["path"].substr(server.getLocation(L)->getPath().size());
    dir = opendir(filename.c_str());
    if(dir != NULL)
    {
        while((pDirent = readdir(dir)) != NULL)
        {
            remove((filename +'/'+ pDirent->d_name).c_str());
        }
    }
    remove(filename.c_str());
}
