#ifndef SERVER_HPP
#define SERVER_HPP
#include "Location.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>


using namespace std;

class Server{
    private:
        int sock_fd;
        struct sockaddr_in serv_addr;
        map<string , string> valueForKey;
        size_t size;
        vector<Location *> locations;

    
    public:
        Server();
        Server(const Server& other);
        ~Server();

        Server& operator=(const Server& ths);

        void    setValue(string key, string value);
        void    setLocation();
        void    setSock_fd();
        void    setServ_addr();
        int     getSock_fd() const;
        string  getValue(string key);
        Location    *getLocation(size_t index) ;
        size_t      getSize() const;
        void        openServer();

};

#endif