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

size_t     ConfigFile::getSocketNum()
{
    size_t i = 0;
    for(size_t j = 0; j < size; j++)
    {
        i += servers[j]->get_sock_v().size();
    }
    return i;
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
        for(size_t j = 0; j < getServer(i)->get_sock_v().size(); j++)
        {
            pollfd _fd;

            _fd.fd = getServer(i)->getSock_fd(j);
            _fd.events = POLLIN | POLLOUT | POLLHUP;
            fds.push_back(_fd);
        }
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
            if ((fds[i].revents & POLLIN) && i < getSocketNum())
                Accept(fds, clients, i);
            else {
                if (fds[i].revents & POLLHUP)
                {
                    cout << "client " << fds[i].fd << "disconnected" << endl;
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    close(clients[i - getSocketNum()].new_sock_fd);
                    clients.erase(clients.begin() + (i - getSocketNum()));
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
                    parseRequest(clients[i - getSocketNum()], buffer);
                    makeResponse(clients[i - getSocketNum()], getRightServer(servers, clients[i - getSocketNum()]));
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