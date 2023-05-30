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

bool isDirectory(const std::string path)
{
    DIR* dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return true;
    }
    return false;
}

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
    // if we dont have a spesific file in that location
    if (req_path.substr(server.getLocation(loc_pos)->getPath().size()).size() == 0 || req_path.at(req_path.size() - 1) == '/')
    {
    //////////////////------ get right index file ------////////////////////
        if (isDirectory(path_to_serve.append(req_path)) && req_path.compare("/"))
        {
            if (req_path.at(req_path.size() - 1) != '/')
                return (GenerateResponse(getRightContent(open(server.getValue("root").append("/").append(server.getValue("default_error")).c_str(), O_RDONLY)), ".html", 301, client), 1);
            return (GenerateResponse(generateAutoIndex(path_to_serve), ".html", 200, client), 0);
        }
        test_file = path_to_serve;
        for (size_t i = 0; i < server.getLocation(loc_pos)->getIndexSize(); i++)
        {
            fd = open(test_file.append("/").append(server.getLocation(loc_pos)->getIndex(i)).c_str(), O_RDONLY);
            if (fd != -1)
                    return (GenerateResponse(getRightContent(fd), getContentType(server.getLocation(loc_pos)->getIndex(i)), 200, client), 0);
            test_file = path_to_serve;
        }
        if (isDirectory(path_to_serve))
        {
            if (!server.getValue("autoindex").compare("ON") || !server.getLocation(loc_pos)->getAutoIndex().compare("ON"))
                return (GenerateResponse(generateAutoIndex(path_to_serve), ".html", 200, client), 0);
        }
        else
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
    if(!filename.compare(client.request["path"]))
        filename = filename.substr(client.request["path"].size());
    if (filename.size() == 0 || !filename.compare("/"))
        filename = '/'+ generateRandomString(10) + getContentTypeExt(client.request["media-type"]);
    cout << getRightRoot(server, L) << endl;
    cout << server.getLocation(L)->get_upload_dir() << endl;
    cout << filename << endl;
    filename = getRightRoot(server, L) + server.getLocation(L)->get_upload_dir() + filename;
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
    filename = getRightRoot(server, L) + client.request["path"].substr(server.getLocation(L)->getPath().size());
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
