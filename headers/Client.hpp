#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include <map>

typedef struct Client
{
    int new_sock_fd;
    int status_code;
    map<string, string> request;
    map<string, string> cookie;
    string sessionID;
    string body;
    string response;
    
}   t_client;


#endif