#include "../headers/ConfigFile.hpp"
#include "../headers/Client.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

bool is_number(const string& s)
{
    string::const_iterator it = s.begin();

    if (*it == '\0')
        return (1);
    while (it != s.end() && isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

int bad_request(t_client& client)
{
    if (!client.request["host"].size())
        return (1);
    else if (client.request["length"].size() && client.request["Transfer-Encoding"].size())
        return (1);
    else if (!is_number(client.request["length"]))
        return (1);
    return (0);
}

int parseRequest(t_client& client, string buffer, vector<Server*> servers)
{
    string key[3] = {"method", "path", "protocol"};
    string str;
    int i = 0;

    stringstream ss(lineToParse("first", buffer));
    while (getline(ss, str, ' '))
        client.request[key[i++]] = str;

    if (client.request["protocol"].compare("HTTP/1.1"))
        return (GenerateResponse(getRightContent(open((const char*)servers[0]->getValue("501_error").c_str(), O_RDONLY)), "text/html", 501, client, ""), 1);

    stringstream ss1(lineToParse("Host", buffer));
    if (getline(ss1, str, ':'))
    {
        if (str.compare("Host"))
        {
            return (GenerateResponse(getRightContent(open((const char*)servers[0]->getValue("400_error").c_str(), O_RDONLY)), "text/html", 400, client, ""), 1);
        }
        if (getline(ss1, str, ':'))
            client.request["host"] = str.substr(1);
        if (getline(ss1, str, ':'))
            client.request["port"] = str;
    }

    if (!client.request["host"].size())
        return (GenerateResponse(getRightContent(open((const char*)servers[0]->getValue("400_error").c_str(), O_RDONLY)), "text/html", 400, client, ""), 1);

    stringstream ss2(lineToParse("Content-Type", buffer));
    if (getline(ss2, str, ' '))
    {
        if (str.compare("Content-Type:"))
            return (GenerateResponse(getRightContent(open(getRightServer(servers, client).getValue("400_error").c_str(), O_RDONLY)), "text/html", 400, client, ""), 1);
        if (getline(ss2, str, ';'))
            client.request["media-type"] = str;
        if (getline(ss2, str, ';'))
            client.request["boundary"] = str.substr(10);
    }
    
    stringstream ss3(lineToParse("Content-Length", buffer));
    if (getline(ss3, str, ' '))
    {
        if (str.compare("Content-Length:"))
            return (GenerateResponse(getRightContent(open(getRightServer(servers, client).getValue("400_error").c_str(), O_RDONLY)), "text/html", 400, client, ""), 1);
        if (getline(ss3, str, ' '))
            client.request["length"] = str;
        if (strtol(getRightServer(servers, client).getValue("max_size").c_str(), 0, 10) < strtol(client.request["length"].c_str(), 0, 10))
            return (GenerateResponse(getRightContent(open(getRightServer(servers, client).getValue("400_error").c_str(), O_RDONLY)), "text/html", 400, client, ""), 1);
    }

    stringstream ss5(lineToParse("Content-Disposition:", buffer));
    if (getline(ss5, str, ' '))
    {
        if (str.compare("Content-Disposition:"))
            return (GenerateResponse(getRightContent(open(getRightServer(servers, client).getValue("400_error").c_str(), O_RDONLY)), "text/html", 400, client, ""), 1);
        getline(ss5, str, ';');
        getline(ss5, str, ';');
        if ( getline(ss5, str, ';'))
            client.request["filename"] = str.substr(11, str.size() - 12);
    }

    stringstream ss4(lineToParse("Transfer-Encoding", buffer));
    if (getline(ss4, str, ' '))
    {
        if (str.compare("Transfer-Encoding:"))
            return (GenerateResponse(getRightContent(open(getRightServer(servers, client).getValue("400_error").c_str(), O_RDONLY)), "text/html", 400, client, ""), 1);
        if (getline(ss4, str, ' '))
            client.request["Transfer-Encoding"] = str;
    }

    stringstream ss6(lineToParse("Cookie", buffer));
    if (getline(ss6, str, ' '))
    {
        if (str.compare("Cookie:"))
            return (GenerateResponse(getRightContent(open(getRightServer(servers, client).getValue("400_error").c_str(), O_RDONLY)), "text/html", 400, client, ""), 1);
        if (getline(ss6, str, ';'))
            client.cookie[str.substr(0, str.find("="))] = str.substr(str.find("="));
    }
    if (bad_request(client))
        return (GenerateResponse(getRightContent(open(getRightServer(servers, client).getValue("400_error").c_str(), O_RDONLY)), "text/html", 400, client, ""), 1);
    return (0);
}
