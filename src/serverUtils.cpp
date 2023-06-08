#include "../headers/ServerUtils.hpp"

unsigned int ft_inet_addr(std::string str)
{
    int i, j;
    char arr[4];

    i = 0;
    j = 0;
    while (str.at(i) != '\0' && str.at(i) != '.')
    {
        if (j == 4)
            throw std::invalid_argument("wrong ip input value");
        i = str.find('.');
        arr[j] = stoi(str.substr(0, i));
        while (str.at(0) != '.' && str.at(0) != '\0')
            str.erase(0,1);
        if (str.at(0) == '.')
            str.erase(0,1);
        j++;
    }

    return *(unsigned int *)arr;
}

void    Accept(vector<pollfd>& fds, vector<t_client>& clients, size_t i)
{
   t_client client;
   pollfd   fd;
   int      clientFd;

    fcntl(fds[i].fd, F_SETFL, O_NONBLOCK);
    if ((clientFd = accept(fds[i].fd ,NULL ,NULL)) < 0)
        throw runtime_error("couldn't accpet client");

    fd.fd = clientFd;
    fd.events = POLLIN | POLLOUT;
    fds.push_back(fd);
    client.new_sock_fd = clientFd;
    client.body.clear();
    clients.push_back(client);
}
