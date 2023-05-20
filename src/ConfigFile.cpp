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

    for (size_t i = 0; i < size; i++)
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
        clients.shrink_to_fit();
        if(poll(fds.data(), fds.size(), -1) < 0)
        {
            //error
        }
        for (size_t i = 0; i < fds.size(); i++)
        {
            if ((fds[i].revents & POLLIN) && i < size)
                Accept(fds, clients, i);
            else {
                if (fds[i].revents & POLLHUP)
                {
                    cout << "client " << fds[i].fd << "disconnected" << endl;
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    close(clients[i - size].new_sock_fd);
                    clients.erase(clients.begin() + (i - size));
                    // delete client && erase client
                    i--;
                    continue;
                }
                if (fds[i].revents & POLLIN)
                {
                    //manage request && create response
                    bzero(buffer, 1024);
                    read(fds[i].fd, &buffer, 1023);
                    //cout << buffer <<endl;
                    parseRequest(clients[i - size], buffer);
                    //fillBody(clients[i - size], buffer);
                    chunkedToNormal(clients[i - size], buffer);
                    //makeResponse(clients[i - size], getRightServer(servers, clients[i - size]));
                    // cout << clients[i - size].request["lenght"] << endl;
                }
                if (fds[i].revents & POLLOUT)
                {
                    // send response
                }
            }
        }
    }
}