#include "../headers/ConfigFile.hpp"

ConfigFile::ConfigFile(){
    cout << "ConfigFile default constructor called" << endl;
    size = 0;
}

ConfigFile::ConfigFile(const ConfigFile& other)
{
    //cout << "ConfigFile copy constructor called" << endl;
    *this = other;
}

ConfigFile& ConfigFile::operator=(const ConfigFile& rhs)
{
    this->size = rhs.size;
    for (size_t i = 0; i < rhs.servers.size(); i++)
        this->servers[i] = rhs.servers[i];
    return *this;
}

ConfigFile::~ConfigFile(){
    //cout << "ConfigFile destructor called" << endl;
    servers.clear();
}

Server *ConfigFile::getServer(size_t index){
    if (index >= size)
        throw out_of_range("ConfigFile::getServer");
    return(servers[index]);
}

size_t ConfigFile::getSize() const{
    return (size);
}

void ConfigFile::setServer(){
    Server* srv = new Server();
    servers.push_back(srv);
    size++;
}

void ConfigFile::run_servers(){
    vector<pollfd> fds;
    vector<Client> clients;
    char buffer[1024];
    string test;

    bzero(buffer, 1024);
    for (size_t i = 0; i < getSize(); i++)
    {
        getServer(i)->openServer();
        pollfd _fd;

        _fd.fd = getServer(i)->getSock_fd();
        _fd.events = POLLIN | POLLOUT | POLLHUP;
        fds.push_back(_fd);
    }
    while (true)
    {
        fds.shrink_to_fit();
        if (poll(fds.data(), fds.size(), -1) < 0)
        {
            //error
        }
        for (size_t i = 0; i < fds.size(); i++)
        {
            cout << "test2" << endl;
            if ((fds[i].revents & POLLIN) && i < size)
                Accept(fds, clients, i);
            else {
                cout << "test3" << endl;
                if (fds[i].revents & POLLHUP)
                {
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    // delete client && erase client
                    i--;
                    continue;
                }
                if (fds[i].revents & POLLIN)
                {
                    cout << "test4" << endl;
                    //manage request && create response
                    read(fds[i].fd, buffer, 1024);
                    cout << buffer << endl;
                    parseRequest(clients[i - size], buffer);
                    // test = lineToParse("Host" ,buffer);
                    // cout << clients[i - size].request["host"] << endl;
                }
                if (fds[i].revents & POLLOUT)
                {
                    // send response
                }
            }
        }
    }
}