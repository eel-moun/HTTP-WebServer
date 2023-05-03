#include <iostream>
#include <vector>
#include <exception>
#include "Server.hpp"

using namespace std;

class ConfigFile{
    private:
        size_t size;
        vector<Server> servers;
    public:
        ConfigFile();
        ~ConfigFile();

        Server getServer(size_t index) const;
        void setServer(Server &srv);
        size_t getSize() const;
};