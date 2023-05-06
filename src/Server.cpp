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
