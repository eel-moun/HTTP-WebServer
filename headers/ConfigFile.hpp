#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "Server.hpp"
#include <exception>
#include "Client.hpp"
#include "ServerUtils.hpp"

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
};
string lineToParse(string key, string buffer);
#endif