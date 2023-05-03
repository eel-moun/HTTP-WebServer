#include <iostream>
#include <vector>
#include "Location.hpp"

using namespace std;

class Server{
    private:
        string listen;
        string host;
        string server_name;
        string error_page400;
        string error_page402;
        string error_page403;
        string error_page404;
        string error_page500;
        string error_page501;
        size_t size;
        vector<Location> locations;
    
    public:
        Server();
        Server(const Server& other);
        ~Server();

        Server& operator=(const Server& ths);

        void    setListen(string port);
        void    setHost(string ip);
        void    setServerName(string name);
        void    setErrorPage(string path,unsigned int index);
        void    setLocation(Location &local);
        string  getListen() const;
        string  getHost() const;
        string  getServerName() const;
        string  getErrorPage(unsigned int index) const;
        Location    getLocation(size_t index) const;
        size_t      getSize()const;

};