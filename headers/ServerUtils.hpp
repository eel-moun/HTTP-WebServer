#ifndef SERVERUTILS_HPP
#define SERVERUTILS_HPP
#include <iostream>
#include <exception>
#include "Server.hpp"
#include "Client.hpp"

void            Accept(vector<pollfd>& fds, vector<t_client>& clients, size_t i);
int             getRightLocation(string req_path, Server server);
Server          getRightServer(vector<Server *> Servers, t_client client);
string          getRightContent(int fd);
string          getRightRoot(Server server, int loc_pos);

#endif