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
        vector<int> sock_fd;
        vector<struct sockaddr_in*> serv_addr;
        vector<string> listens;
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
        void    setServ_addr();
        int     getSock_fd(int index) const;
        string  getValue(string key);
        vector<string>  get_listens();
        Location    *getLocation(size_t index) ;
        size_t      getSize() const;
        void        openServer();
        void        set_listens(string value);
        vector<int> get_sock_v();
};

#endif