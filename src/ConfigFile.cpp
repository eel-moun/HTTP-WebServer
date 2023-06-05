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
    int r;
    char buffer[1024];
    size_t w = 0;

    for (size_t i = 0; i < size; i++)
    {
        getServer(i)->openServer();
        for (size_t j = 0; j < getServer(i)->get_sock_v().size(); j++)
        {
            pollfd _fd;

            fcntl(getServer(i)->getSock_fd(j), F_SETFL, O_NONBLOCK);
            _fd.fd = getServer(i)->getSock_fd(j);
            _fd.events = POLLIN | POLLOUT | POLLHUP;
            fds.push_back(_fd);
        }
    }
    while (true)
    {
        fds.shrink_to_fit();
        clients.shrink_to_fit();
        if (poll(fds.data(), fds.size(), -1) < 0)
        {
            cout << strerror(errno) << endl;
            exit(1);
        }
        for (size_t i = 0; i < fds.size(); i++)
        {
            if ((fds[i].revents & POLLIN) && i < getSocketNum())
            {
                Accept(fds, clients, i);
            }
            else
            {
                if (fds[i].revents & POLLHUP)
                {
                    cout << "client " << fds[i].fd << " disconnected" << endl;
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    clients.erase(clients.begin() + (i - getSocketNum()));
                    i--;
                    continue;
                }
                if (fds[i].revents & POLLIN && !clients[i - getSocketNum()].response.size())
                {
                    bzero(buffer, 1024);
                    r = read(fds[i].fd, &buffer, 1023);
                    if (!clients[i - getSocketNum()].body.size())
                        parseRequest(clients[i - getSocketNum()], string(buffer, r), servers);
                    if (!clients[i - getSocketNum()].response.size())
                        fillBody(clients[i - getSocketNum()], string(buffer,r));
                    checkRedir(clients[i - getSocketNum()], getRightServer(servers, clients[i - getSocketNum()]));
                    if (!clients[i - getSocketNum()].response.size())
                        makeResponse(clients[i - getSocketNum()], getRightServer(servers, clients[i - getSocketNum()]));
                }
                    if (fds[i].revents & POLLOUT && clients[i - getSocketNum()].response.size())
                {
                    while (w < clients[i - getSocketNum()].response.size())
                    {
                        r = write(fds[i].fd, clients[i - getSocketNum()].response.c_str() + w , clients[i - getSocketNum()].response.size() - w);
                        if (r != -1)
                            w += r;
                    }
                    clients[i - getSocketNum()].response.clear();
                    w = 0;
                }
            }
        }
    }
}