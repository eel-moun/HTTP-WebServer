#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include <map>

typedef struct Client
{
    int new_sock_fd;
    std::map<std::string, std::string> request;
    string body;
    std::string response;
    
}   t_client;


#endif