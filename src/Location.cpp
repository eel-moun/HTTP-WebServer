#include "../headers/Location.hpp"

Location::Location(){
    cout << "Location default constructor called" << endl;
}

Location::~Location(){
    cout << "Location Destructor called" << endl;
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