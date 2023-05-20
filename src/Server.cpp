#include "../headers/Server.hpp"
#include "../headers/ServerUtils.hpp"

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
    setServ_addr();
}

void Server::setServ_addr(){
    int opt;

    opt = 1;
    for (size_t i = 0; i < listens.size(); i++)
    {

        struct sockaddr_in* addr = new struct sockaddr_in;
        sock_fd.push_back(socket(AF_INET, SOCK_STREAM, 0));

        if (sock_fd[i] < 0)
            throw runtime_error("socket not initialised correctly");

        if (setsockopt(sock_fd[i], SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
            throw runtime_error("Server::setSock_fd sockopt not set correctly");
        
        if (fcntl(sock_fd[i], F_SETFL, O_NONBLOCK) < 0)
            throw runtime_error("Error while setiing O_NONOBLOCK...");

        addr->sin_family = AF_INET;
        if (!this->getValue("host").compare("localhost")) // possible_error
            addr->sin_addr.s_addr = INADDR_ANY;
        else
            addr->sin_addr.s_addr = ft_inet_addr(this->getValue("host")); // possible_error
        cout << listens[i] << endl;
        addr->sin_port = htons(stoi(listens[i]));


        if (bind(sock_fd[i], reinterpret_cast<struct sockaddr*>(addr), sizeof(struct sockaddr_in)) < 0)
        {
        cout << strerror(errno) << endl;
        throw runtime_error("Server::setServ_addr couldn't bind");
        }

        if (listen(sock_fd[i], 10) < 0)
            throw runtime_error("Server::setServ_addr couldn't listen");
        serv_addr.push_back(addr);
    }
}

int     Server::getSock_fd(int index) const
{
    return (sock_fd[index]);
}

void    Server::set_listens(string value)
{
    string token;

    if (this->listens.size())
        throw invalid_argument("Location::set_return key already initialised");
    stringstream ss(value);
    while (getline(ss, token, ' '))
        this->listens.push_back(token);
}

vector<string>  Server::get_listens()
{
    return (this->listens);
}

vector<int>  Server::get_sock_v()
{
    return (sock_fd);
}