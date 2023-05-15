#include "../headers/Location.hpp"

Location::Location(){
    //cout << "Location default constructor called" << endl;
}

Location::~Location(){
    //cout << "Location Destructor called" << endl;
}

Location::Location(const Location& other)
{
    //cout << "Location copy constructor called" << endl;
    *this = other;
}

Location&   Location::operator=(const Location& rhs)
{
    //cout << "Location copy assignment operator called" << endl;
    this->path = rhs.path;
    this->index = rhs.index;
    return *this;
}

void Location::setPath(string _path){
    if(path.size())
    {
        cout << path.size() << endl;
        cout << path << endl;
        throw invalid_argument("Location::setPath key already initialised");
    }
    this->path = _path;
}

void Location::setIndex(string _index){
    if(index.size())
        throw invalid_argument("Location::setPath key already initialised");
    this->index = _index;
}

string Location::getPath() const{
    return path;
}

string Location::getIndex() const{
    return index;
}

void    Location::setFile(string _file)
{
    this->files.push_back(_file);
}

string  Location::getFile(size_t index)
{
    if (index >= this->files.size())
        throw out_of_range("Server::getLocation");
    return (files[index]);
}

size_t  Location::getSize()
{
    return (this->files.size());
}
