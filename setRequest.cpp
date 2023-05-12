#include "headers/ConfigFile.hpp"
#include "headers/Client.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

void    parseRequest(t_client& client, string buffer)
{
    string key[8] = {"method", "path", "protocol", "host", "port", "media-type", "charset", "lenght"};
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
            client.request.insert(pair<string, string>(key[i++], str.substr(1)));
        if (getline(ss1, str, ':'))
            client.request.insert(pair<string, string>(key[i++], str));
    }

    stringstream ss2(lineToParse("Content-Type", buffer));
    if (getline(ss2, str, ' '))
    {
        cout << "||||" << str << "||||" << endl;
        if (str.compare("Content-Type:"))
            throw runtime_error("unwanted value1");
        if (getline(ss2, str, ';'))
            client.request.insert(pair<string, string>(key[i++], str));
        if (getline(ss2, str, ';'))
            client.request.insert(pair<string, string>(key[i++], str.substr(1)));
    }
    
    str = lineToParse("Content-Length", buffer);
    if (!str.compare(""))
        return;
    if (str.compare("Content-Length"))
        throw runtime_error("unwanted value");
    client.request.insert(pair<string, string>(key[i++], str.substr(16)));

}
