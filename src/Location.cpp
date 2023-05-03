#include "../headers/Location.hpp"

Location::Location(){
    cout << "Location default constructor called" << endl;
}

Location::~Location(){
    cout << "Location Destructor called" << endl;
}

Location::Location(const Location& other)
{
    cout << "Location copy constructor called" << endl;
    *this = other;
}

Location&   Location::operator=(const Location& rhs)
{
    cout << "Location copy assignment operator called" << endl;
    this->path = rhs.path;
    this->index = rhs.index;
}

void Location::setPath(string path){
    this->path = path;
}

void Location::setIndex(string index){
    this->index = index;
}

string Location::getPath() const{
    return path;
}

string Location::getIndex() const{
    return index;
}