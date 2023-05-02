#include <iostream>
#include <vector>
using namespace std;

class configfile{
    private:
        size_t size;
        vector<server> servers;
    public:
        configfile();
        ~configfile();

        server getServer(size_t index);
        void setServer(server &srv);
        size_t getSize()const;
};

class server{
    private:
        string listen;
        string host;
        string server_name;
        string error_page400;
        string error_page402;
        string error_page403;
        string error_page404;
        string error_page500;
        string error_page501;
        vector<location> locations;
    
    public:
        server();
        ~server();

        void    setListen(string port);
        void    setHost(string ip);
        void    setServerName(string name);
        void    setErrorPage(string path,unsigned int index);
        void    setLocation(location &local);
        string  getListen() const;
        string  getHost() const;
        string  getServerName() const;
        string  getErrorPage(unsigned int index) const;
        location    getLocation(size_t index) const;

};

class location{
    private:
        string path;
        string index;
    
    public:
        location();
        ~location();

        void setPath(string path);
        void setIndex(string index);
        string getPath() const;
        string getIndex() const;
};