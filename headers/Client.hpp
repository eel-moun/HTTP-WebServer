#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include <map>

typedef struct Client
{
    int new_sock_fd;
    int status_code;
    int timeout;
    map<string, string> request;
    map<string, string> cookie;
    string sessionID;
    string body;
    string response;
    string buffer;
    bool   header_removed;
    bool   finished_reading;
    bool   chunked_len_set;
    size_t chunked_len;
    size_t total_read;
    size_t w;
    
}   t_client;


#endif