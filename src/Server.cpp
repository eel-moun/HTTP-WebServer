#include "../headers/Server.hpp"
#include "../headers/ServerCreation.hpp"

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

void    Server::openServer()
{
    setSock_fd();
    setServ_addr();
}

void Server::setSock_fd(){
    int opt;

    opt = 1;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0)
        throw runtime_error("socket not initialised correctly");

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
        throw runtime_error("Server::setSock_fd sockopt not set correctly");
    
    if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) < 0)
        throw runtime_error("Error while setiing O_NONOBLOCK...");
}

void Server::setServ_addr(){

    serv_addr.sin_family = AF_INET;
    
    if (!this->getValue("host").compare("localhost")) // possible_error
        serv_addr.sin_addr.s_addr = INADDR_ANY;
    else
        serv_addr.sin_addr.s_addr = ft_inet_addr(this->getValue("host")); // possible_error
    serv_addr.sin_port = htons(stoi(this->getValue("listen")));

    if (bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        throw runtime_error("Server::setServ_addr couldn't bind");

    if (listen(sock_fd, 10) < 0)
        throw runtime_error("Server::setServ_addr couldn't listen");
}

int     Server::getSock_fd() const
{
    return (this->sock_fd);
}
