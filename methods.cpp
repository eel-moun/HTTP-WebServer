#include "headers/ConfigFile.hpp"

int    GetMethod(t_client& client, Server server)
{
    string req_path;
    string path_to_serve;
    string test_file;
    int fd;

    req_path = client.request["path"].substr(0, req_path.find("?"));
    int loc_pos = getRightLocation(req_path, server);
    path_to_serve = getRightRoot(server, loc_pos);
    if (req_path.substr(server.getLocation(loc_pos)->getPath().size()).size() == 0)
    {
    //////////////////------ get right index file ------////////////////////
        test_file = path_to_serve;
        for (size_t i = 0; i < server.getLocation(loc_pos)->getIndexSize(); i++)
        {
            fd = open(test_file.append("/").append(server.getLocation(loc_pos)->getIndex(i)).c_str(), O_RDONLY);
            if (fd != -1)
                    return (GenerateResponse(getRightContent(fd), getContentType(server.getLocation(loc_pos)->getIndex(i)), 200, client), 0);
            test_file.substr(0, test_file.find_last_of("/"));
        }
        return (GenerateResponse("", "", 404, client), 1);
    }
    else
    {
        ////////////////------  check file   ------//////////////////
        if (server.getLocation(loc_pos)->getPath().compare("/"))
            path_to_serve = path_to_serve.append(req_path.substr(server.getLocation(loc_pos)->getPath().size()));
        else
            path_to_serve = path_to_serve.append(req_path);

        fd = open(path_to_serve.c_str(), O_RDONLY);
        if (fd != -1)
            return (GenerateResponse(getRightContent(fd), getContentType(req_path), 200, client), 0);
        throw runtime_error("error 404 not found");
    }
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
    cout<< client.request["media-type"] << endl;
    if (filename.size() == 0 || !filename.compare("/"))
        filename = '/'+ generateRandomString(10) + "." +client.request["Content-Type"];

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
    /*if(!count(server.getLocation(L)->getAllowedMethod().begin(),server.getLocation(L)->getAllowedMethod().end(),"DELETE"))
    {
            //error METHOD NOT ALLOWED
    }*/
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
