#include "headers/ConfigFile.hpp"
#include "headers/Client.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>

/*
There are many different types of errors that can occur when handling a GET request. Here are some examples:

400 Bad Request: The request is malformed and cannot be understood by the server.
401 Unauthorized: The client is not authenticated and cannot access the requested resource.
403 Forbidden: The client is authenticated, but does not have permission to access the requested resource.
404 Not Found: The requested resource could not be found on the server.
405 Method Not Allowed: The requested resource does not support the HTTP method used in the request (e.g. GET vs POST).
406 Not Acceptable: The client requested a content type that is not supported by the server.
500 Internal Server Error: The server encountered an unexpected error while processing the request.

These are just a few examples of the many possible errors that can occur during a GET request.
In general, it's a good idea to handle errors gracefully by returning an appropriate error status code
and a human-readable error message in the response body.*/

string getContentType(const string& path)
{
    if (path.rfind(".html") != string::npos)
        return ("text/html");

    else if (path.rfind(".css") != string::npos)
        return ("text/css");

    else if (path.rfind(".js") != string::npos)
        return ("text/javascript");

    else if (path.rfind(".png") != string::npos)
        return ("image/png");

    else if (path.rfind(".jpg") != string::npos)
        return ("image/jpg");

    else if (path.rfind(".gif") != string::npos)
        return ("image/gif");

    else if (path.rfind(".svg") != string::npos)
        return ("image/svg+xml");

    else if (path.rfind(".pdf") != string::npos)
        return ("application/pdf");

    else if (path.rfind(".zip") != string::npos)
        return ("application/zip");

    else if (path.rfind(".gz") != string::npos)
        return ("application/gzip");

    else if (path.rfind(".mp3") != string::npos)
        return ("audio/mpeg");

    else if (path.rfind(".mp4") != string::npos)
        return ("video/mp4");

    else if (path.rfind(".mkv") != string::npos)
        return ("video/x-matroska");

    else if (path.rfind(".xml") != string::npos)
        return ("application/xml");

    else if (path.rfind(".json") != string::npos)
        return ("application/json");

    else
        return ("application/octet-stream");
}

const string    getErrorByCode(const int error_code)
{
    switch (error_code)
    {
        case 200:
            return (" OK");
        case 404:
            return (" Page Not Found");
        case 500:
            return (" Internal Server Error");
        case 501:
            return (" Not Implemented");
        case 502:
            return (" Bad Gateway");
        case 503:
            return (" Service Unavailable");
        case 504:
            return (" Gateway Timeout");
        default:
            return (" Unknown Error");
    }
}

void    sendResponse(const string& content, const string& content_type, const int error_code, const int client_fd)
{
    string response;
    stringstream ss;

    ss << "HTTP/1.1 " << error_code << getErrorByCode(error_code) << "\r\n";
    ss << "Content-Type: " << content_type << "\r\n";
    ss << "Content-Length: " << content.size() << "\r\n";
    ss << "Connection: close\r\n\r\n";
    ss << content;
    response = ss.str();
    write(client_fd, response.c_str(), response.size());
}

int  getRightLocation(string req_path, Server server)
{
    string  loc_path;
    int temp = 0;
    int j = -1;
    int count = 0;

    for (size_t i = 0; i < server.getSize(); i++)
    {
        loc_path = server.getLocation(i)->getPath();
        if (loc_path.compare("/"))
            loc_path.append("/");

        for (size_t z = 0; z < req_path.size() && z < loc_path.size() ; z++)
        {
            if (req_path[z] == loc_path[z])
            {
                if (req_path[z] == '/')
                    count++;
            }
            else
            {
                count = 0;
                break;
            }
        }

        if(temp < count)
        {
            temp = count;
            j = i;
            count = 0;
        }
    }

    return (j);
}

void    GetMethod(t_client& client, Server server)
{
    string req_path;
    string buffer;
    string path_to_serve;
    string test_file;
    

    req_path = client.request["path"].substr(0, req_path.find("?")); //  /mok || / || /test

//  5asni n9alab 3la location li m3a request

    int loc_pos = getRightLocation(req_path, server);
    if (loc_pos == -1)
    {
        // if (server.getValue("root").size() && server.getValue("index").size())
        //     path_to_serve = server.getValue("root").append("/").append(server.getValue("index"));
        // else
            throw runtime_error("error 403 forbiden");
    }
    ///////////////////////////////////////////////////////////////////
    else if (req_path.substr(server.getLocation(loc_pos)->getPath().size()).size() == 0)
    {
        ////////////////------ get right root ------//////////////////
        if (server.getLocation(loc_pos)->getRoot().size())
            path_to_serve = server.getLocation(loc_pos)->getRoot();
        else if (server.getValue("root").size())
            path_to_serve = server.getValue("root");
        else
            throw runtime_error("403 forbiden");
    //////////////////------ get right index file ------////////////////////

        test_file = path_to_serve;
        for (size_t i = 0; i < server.getLocation(loc_pos)->getIndexSize(); i++)
        {
            ifstream check_path(test_file.append("/").append(server.getLocation(loc_pos)->getIndex(i)));
            if (check_path.is_open())
            {
                if (getline(check_path, test_file, '\0'))
                {
                    sendResponse(test_file, getContentType(server.getLocation(loc_pos)->getIndex(i)), 200, client.new_sock_fd);
                    return ;
                }
                    
            }
            test_file.substr(0, test_file.find_last_of("/"));
        }
        throw runtime_error("error 404 not found"); 
    }
    else
    {
        ////////////////------ get right root ------//////////////////
        if (server.getLocation(loc_pos)->getRoot().size())
            path_to_serve = server.getLocation(loc_pos)->getRoot();
        else if (server.getValue("root").size())
            path_to_serve = server.getValue("root");
        else
            throw runtime_error("403 forbiden");
        ////////////////------ check file    ------//////////////////
        if (server.getLocation(loc_pos)->getPath().compare("/"))
            path_to_serve = path_to_serve.append(req_path.substr(server.getLocation(loc_pos)->getPath().size()));
        else
            path_to_serve = path_to_serve.append(req_path);
        ifstream check_path(path_to_serve);
        if (check_path.is_open())
        {
            if (getline(check_path, test_file, '\0'))
            {
                sendResponse(test_file, getContentType(req_path), 200, client.new_sock_fd);
                return ;
            }
        }
        throw runtime_error("error 404 not found");
    }
}

// void    PostMethod(t_client& client)
// {

// }

// void    DeleteMethod(t_client& client)
// {}

void    makeResponse(t_client& client, Server server)
{
    string method;

    method = client.request["method"];
    if (method == "GET")
        GetMethod(client, server);
    // else if (method == "POST")
    //     PostMethod(client);
    // else if (method == "DELETE")
    //     DeleteMethod(client);
    //else
        // error code 405 method not allowed
}

Server getRightServer(vector<Server *> servers, t_client& client)
{
    string host = client.request["host"];
    string port = client.request["port"];

    if (servers.size() == 1)
        return (*servers[0]);

    if (!host.size() || !port.size())
        throw runtime_error("no host or port available to check");
    
    for (size_t i = 0; i < servers.size(); i++)
        if (servers[i]->getValue("host") == host && servers[i]->getValue("listen") == port)
            return (*servers[i]);

    throw runtime_error("no server found");
}
