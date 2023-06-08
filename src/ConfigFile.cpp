#include "../headers/ConfigFile.hpp"

ConfigFile::ConfigFile(){
    size = 0;
}

ConfigFile::ConfigFile(const ConfigFile& other)
{
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
    char buffer[10024];

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
        size_t socket_num;
        fds.shrink_to_fit();
        clients.shrink_to_fit();
        if (poll(&fds[0], fds.size(), -1) < 0)
        {
            cout << strerror(errno) << endl;
            exit(1);
        }
        for (size_t i = 0; i < fds.size(); i++)
        {
            socket_num = getSocketNum();
            if ((fds[i].revents & POLLIN) && i < socket_num)
            {
                Accept(fds, clients, i);
            }
            else
            {
                if(i >= socket_num)
                    if(time(NULL) - clients[i - socket_num].timeout > 10)
                        GenerateResponse(getRightContent(open(getRightServer(servers, clients[i - socket_num]).getValue("408_error").c_str(), O_RDONLY)), (string&)"text/html", 408, clients[i - socket_num], "");
                if (fds[i].revents & POLLHUP)
                {
                    disconect(fds, clients, i, socket_num);
                    i--;
                    continue;
                }
                if (fds[i].revents & POLLIN && !clients[i - socket_num].response.size())
                {
                    clients[i - socket_num].timeout = time(NULL);
                    bzero(buffer, 10024);
                    r = read(fds[i].fd, &buffer, 10023);
                    if(r == -1 || r == 0)
                    {
                            disconect(fds, clients, i, socket_num);
                            i--;
                            continue;
                    }
                    if (!clients[i - socket_num].header_removed)
                    {
                        if(!parseRequest(clients[i - socket_num], string(buffer, r), servers))
                            checkRedir(clients[i - socket_num], getRightServer(servers, clients[i - socket_num]));
                    }
                    if (!clients[i - socket_num].response.size() && !clients[i - socket_num].finished_reading)
                        fillBody(clients[i - socket_num], string(buffer,r), getRightServer(servers, clients[i - socket_num]));
                    if (!clients[i - socket_num].response.size() && clients[i - socket_num].finished_reading)
                    {
                        if(clients[i - socket_num].body.size() > (size_t)strtol(getRightServer(servers, clients[i - socket_num]).getValue("max_size").c_str(), 0, 10))
                            GenerateResponse(getRightContent(open(getRightServer(servers, clients[i - socket_num]).getValue("400_error").c_str(), O_RDONLY)), (string&)"text/html", 400, clients[i - socket_num], "");
                        else
                            makeResponse(clients[i - socket_num], getRightServer(servers, clients[i - socket_num]));
                    }
                }
                if (fds[i].revents & POLLOUT && clients[i - socket_num].response.size())
                {
                    clients[i - socket_num].timeout = time(NULL);
                    r = write(fds[i].fd, clients[i - socket_num].response.c_str() + clients[i - socket_num].w , clients[i - socket_num].response.size() - clients[i - socket_num].w);
                    if (r > 0)
                        clients[i - socket_num].w += r;
                    if (r == 0 || r == -1 || clients[i - socket_num].w == clients[i - socket_num].response.size())
                    {
                        disconect(fds, clients, i, socket_num);
                        i--;
                        continue;
                    }
                }
            }
        }
    }
}