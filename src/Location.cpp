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

void Location::setIndex(string _index)
{
    string token;

    stringstream ss(_index);
    while (getline(ss, token, ' '))
        this->index.push_back(token);
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
    string token;

    stringstream ss(_method);
    while (getline(ss, token, ' '))
        this->allowed_method.push_back(token);
}

vector<string>  Location::getAllowedMethod()
{
    return (allowed_method);
}

void    Location::setAutoIndex(string _auto_index)
{
    if (this->auto_index.size())
        throw invalid_argument("Location::setAutoIndex");
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

void    Location::set_upload_dir(string _upload_dir)
{
    this->upload_dir = _upload_dir;
}

string  Location::get_upload_dir()
{
    return (this->upload_dir);
}

void    Location::set_return(string value)
{
    string token;

    if (this->_return.size())
        throw invalid_argument("Location::set_return key already initialised");
    this->_return = value;
}

string Location::get_return()
{
    return (this->_return);
}

void    Location::set_cgi_path(string path)
{
    if (this->cgi_ext.size())
        throw invalid_argument("Location::set_cgi_path key already initialised");
    this->cgi_path = path;
}

string  Location::get_cgi_path()
{
    return (this->cgi_path);
}

void    Location::set_cgi_ext(string ext)
{
    if (this->cgi_ext.size())
        throw invalid_argument("Location::set_cgi_ext key already initialised");
    this->cgi_ext = ext;
}

string  Location::get_cgi_ext()
{
    return (this->cgi_ext);
}
