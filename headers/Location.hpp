#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Location{
    private:
        string path;
        vector<string> index;
        string root;
        string auto_index;
        vector<string> allowed_method;
    
    public:
        Location();
        Location(const Location& other);
        ~Location();

        Location& operator=(const Location& rhs);

        void    setPath(string path);
        void    setIndex(string index);
        void    setRoot(string _root);
        void    setAutoIndex(string _auto_index);
        void    setAllowedMethod(string _method);
        string  getAllowedMethod(size_t index);
        string  getAutoIndex();
        string  getPath();
        string  getIndex(size_t index);
        string  getRoot();
        size_t  getIndexSize();
};
#endif