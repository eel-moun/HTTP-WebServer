#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "Server.hpp"
#include <exception>
#include <algorithm>
#include "Client.hpp"
#include "ServerUtils.hpp"
#include <dirent.h>
#include <random>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <algorithm>
#include <stdlib.h>


using namespace std;

class ConfigFile{
    private:
        size_t size;
        vector<Server*> servers;
    public:
        ConfigFile();
        ConfigFile(const ConfigFile& other);
        ~ConfigFile();

        ConfigFile& operator=(const ConfigFile& rhs);

        Server *getServer(size_t index);
        void setServer();
        void run_servers();
        size_t getSize() const;
        size_t    getSocketNum();
};

void            parseRequest(t_client& client, string buffer, vector<Server*> servers);
string          lineToParse(string key, string buffer);
void            makeResponse(t_client& client, Server server);
int             GetMethod(t_client& client, Server server);
void            PostMethod(t_client& client, Server& server);
void            DeleteMethod(t_client& client, Server server);
void            GenerateResponse(const string& content, const string& content_type, const int status_code, t_client& client);

string          getContentType(const string& path);
string          getContentTypeExt(const string& path);
string          generateRandomString(int length);
void            normalBody(t_client& client,string buffer);
void            chunkedToNormal(t_client& client, string buffer);
void            fillBody(t_client& client,string buffer);

#endif