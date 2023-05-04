#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "Server.hpp"
#include <exception>

using namespace std;

class ConfigFile{
    private:
        size_t size;
        vector<Server> servers;
    public:
        ConfigFile();
        //ConfigFile(const ConfigFile& other);
        ~ConfigFile();

        //ConfigFile& operator=(const ConfigFile& rhs);

        Server getServer(size_t index) const;
        void setServer();
        size_t getSize() const;
};
#endif