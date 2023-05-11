#include "headers/ConfigFile.hpp"
#include "headers/Client.hpp"

string test()
{}

void    parseRequest(t_client& client)
{
    string key[8] = {"method", "path", "protocol", "host", "port", "media-type", "charset", "lenght"};
    string str;
    int i = 0;

    stringstream ss(test("first"));
    while (getline(ss, str, ' '))
        client.request.insert(pair<string, string>(key[i++], str));

    stringstream ss(test("host"));
    if (getline(ss, str, ':'))
    {
        if (str.compare("Host"))
            throw runtime_error("unwanted value");
        if (getline(ss, str, ':'))
            client.request.insert(pair<string, string>(key[i++], str.substr(1)));
        if (getline(ss, str, ':'))
            client.request.insert(pair<string, string>(key[i++], str));
    }

    stringstream ss(test("content-type"));
    if (getline(ss, str, ' '))
    {
        if (str.compare("Content-Type:"))
            throw runtime_error("unwanted value");
        if (getline(ss, str, ';'))
            client.request.insert(pair<string, string>(key[i++], str));
        if (getline(ss, str, ';'))
            client.request.insert(pair<string, string>(key[i++], str.substr(1)));
    }
    
    str = test("content-length");
    if (str.compare("Content-Length"))
        throw runtime_error("unwanted value");
    client.request.insert(pair<string, string>(key[i++], str.substr(16)));

}
