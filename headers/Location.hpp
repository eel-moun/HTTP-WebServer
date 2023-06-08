#ifndef LOCATION_HPP
#define LOCATION_HPP
#include <iostream>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

class Location{
    private:
        string path;
        vector<string> index;
        string root;
        string auto_index;
        string upload_dir;
        string cgi_ext;
        vector<string> _return;
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
        void    set_upload_dir(string _upload_dir);
        void    set_return(string value);
        void    set_cgi_ext(string ext);
        string  get_cgi_ext();
        string  getAutoIndex();
        string  getPath();
        string  getIndex(size_t index);
        string  getRoot();
        size_t  getIndexSize();
        string  get_upload_dir();
        vector<string>  getAllowedMethod();
        vector<string> get_return();
};
#endif