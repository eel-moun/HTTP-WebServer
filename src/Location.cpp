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

string Location::getPath(){
    return path;
}

void Location::setIndex(string _index){
    this->index.push_back(_index);
}

string Location::getIndex(size_t _index){
    return this->index[_index];
}

void    Location::setRoot(string _root)
{
    if (this->root.size())
        throw invalid_argument("Location::setPath key already initialised");
    this->root = _root;
}

string  Location::getRoot()
{
    return (this->root);
}

void    Location::setAllowedMethod(string _method)
{
    this->allowed_method.push_back(_method);
}

string  Location::getAllowedMethod(size_t index)
{
    return (this->allowed_method[index]);
}

void    Location::setAutoIndex(string _auto_index)
{
    if (this->auto_index.size())
        throw invalid_argument("Location::setAutoIndex key already initialised");
    this->auto_index = _auto_index;
}

string  Location::getAutoIndex()
{
    return (this->auto_index);
}

size_t  Location::getIndexSize()
{
    return (this->index.size());
}
