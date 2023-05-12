#include "headers/ConfigFile.hpp"
#include "headers/Client.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

void    parseRequest(t_client& client, string buffer)
{
    string key[3] = {"method", "path", "protocol"};
    string str;
    int i = 0;

    stringstream ss(lineToParse("first", buffer));
    while (getline(ss, str, ' '))
        client.request.insert(pair<string, string>(key[i++], str));

    stringstream ss1(lineToParse("Host", buffer));
    if (getline(ss1, str, ':'))
    {
        if (str.compare("Host"))
            throw runtime_error("unwanted value");
        if (getline(ss1, str, ':'))
            client.request.insert(pair<string, string>("host", str.substr(1)));
        if (getline(ss1, str, ':'))
            client.request.insert(pair<string, string>("port", str));
    }

    stringstream ss2(lineToParse("Content-Type", buffer));
    if (getline(ss2, str, ' '))
    {
        if (str.compare("Content-Type:"))
            throw runtime_error("unwanted value1");
        if (getline(ss2, str, ';'))
            client.request.insert(pair<string, string>("media-type", str));
        if (getline(ss2, str, ';'))
            client.request.insert(pair<string, string>("charset", str.substr(1)));
    }
    
    stringstream ss3(lineToParse("Content-Length", buffer));
    if (getline(ss3, str, ' '))
    {
        if (str.compare("Content-Length:"))
            throw runtime_error("unwanted value");
        if (getline(ss3, str, ' '))
            client.request.insert(pair<string, string>("lenght", str));
    }

}
