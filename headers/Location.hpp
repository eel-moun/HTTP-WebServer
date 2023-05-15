#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Location{
    private:
        string path;
        string index;
        vector<string> files;
    
    public:
        Location();
        Location(const Location& other);
        ~Location();

        Location& operator=(const Location& rhs);

        void    setPath(string path);
        void    setIndex(string index);
        void    setFile(string file);
        string  getPath() const;
        string  getIndex() const;
        string  getFile(size_t index);
        size_t  getSize();
};
#endif