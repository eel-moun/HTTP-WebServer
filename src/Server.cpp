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
    this->size = rhs.size;
    this->valueForKey = rhs.valueForKey;
    this->locations = rhs.locations;
    return *this;
}

Server::~Server(){
    cout << "Server default destructor called " << endl;
    locations.clear();
}

void Server::setValue(string key, string value)
{
    if(valueForKey.find(key) != valueForKey.end())
        throw invalid_argument("Server::setValue key already initialised");
    valueForKey.insert(pair<string, string>(key, value));
}

string Server::getValue(string key) {
    return valueForKey[key];
}

void Server::setLocation(){
    Location *local = new Location();
    locations.push_back(local);
    size++;
}

Location *Server::getLocation(size_t index){
    if (index >= size)
        throw out_of_range("Server::getLocation");
    return locations[index];
}

size_t Server::getSize() const{
    return size;
}

#include <fcntl.h>

void    Server::openServer()
{
    int sock_fd, new_sock_fd, len_serv_addr, opt;

    struct sockaddr_in serv_addr;
    len_serv_addr = sizeof(serv_addr);
    opt = 1;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        strerror(errno);
        return ;
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        strerror(errno);
        close(sock_fd);
        return ;
    }

    if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) < 0) {
        throw std::runtime_error("Error while setiing O_NONOBLOCK...");
    }

    // 5asna n7ado no9at o ngeriw localhost nbadloh l default 


    serv_addr.sin_family = AF_INET;
    if (!this->getValue("host").compare("localhost"))
        serv_addr.sin_addr.s_addr = INADDR_ANY;
    else
        // serv_addr.sin_addr.s_addr = this->getValue("host");
    serv_addr.sin_port = htons(stoi(this->getValue("listen")));

    if (bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        strerror(errno);
        close(sock_fd);
        return;
    }

    if (listen(sock_fd, 10))
    {
        strerror(errno);
        close(sock_fd);
        return ;
    }

    if ((new_sock_fd = accept(sock_fd, (struct sockaddr*)&serv_addr, (socklen_t*)&len_serv_addr)))
    {
        strerror(errno);
        close(sock_fd);
        return ;
    }

    

}
