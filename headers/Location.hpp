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
    
    public:
        Location();
        Location(const Location& other);
        ~Location();

        Location& operator=(const Location& rhs);

        void setPath(string path);
        void setIndex(string index);
        string getPath() const;
        string getIndex() const;
};
#endif