#include <iostream>
#include <vector>

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