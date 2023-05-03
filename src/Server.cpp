#include "../headers/Server.hpp"

Server::Server(){
    cout << "Server default constructor called" << endl;
    size = 0;
}

Server::Server(const Server& other)
{
    cout << "Server copy constructor called" << endl;
    *this = other;
}

Server& Server::operator=(const Server& rhs)
{
    cout << "Server copy assignment operator called" << endl;
    this->listen = rhs.listen;
    this->host = rhs.host;
    this->server_name = rhs.server_name;
    this->error_page400 = rhs.error_page400;
    this->error_page402 = rhs.error_page403;
    this->error_page403 = rhs.error_page403;
    this->error_page404 = rhs.error_page404;
    this->error_page500 = rhs.error_page500;
    this->error_page501 = rhs.error_page501;
    this->size = rhs.size;
    this->locations = rhs.locations;
}

Server::~Server(){
    cout << "Server default destructor called " << endl;
    locations.clear();
}

void Server::setListen(string port){
    listen = port;
}

void Server::setHost(string ip){
    host = ip;
}

void Server::setServerName(string name){
    server_name = name;
}

void Server::setErrorPage(string path, unsigned int index){
    switch (index)
    {
        case 400:
            error_page400 = path;
            break;
        case 402:
            error_page402 = path;
            break;
        case 403:
            error_page403 = path;
            break;
        case 404:
            error_page404 = path;
            break;
        case 500:
            error_page500 = path;
            break;
        case 501:
            error_page501 = path;
            break;
        default:
            cout << "error page not supported" << endl;
        break;
    }
}

void Server::setLocation(Location &local){
    locations.push_back(local);
    size++;
}

string Server::getListen() const {
    return listen;
}

string Server::getHost() const {
    return host;
}

string Server::getServerName() const{
    return server_name;
}

string Server::getErrorPage(unsigned int index) const{
        switch (index)
    {
        case 400:
            return error_page400;
        case 402:
            return error_page402;
        case 403:
            return error_page403;
        case 404:
            return error_page404;
        case 500:
            return error_page500;
        case 501:
            return error_page501;
        default:
            cout << "error page not available" << endl;
            return NULL;
    }
}

Location Server::getLocation(size_t index) const{
    if (index > size)
        throw out_of_range("Server::getLocation");
    return locations[index];
}

size_t Server::getSize() const{
    return size;
}
