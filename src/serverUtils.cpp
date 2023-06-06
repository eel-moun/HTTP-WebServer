#include "../headers/ServerUtils.hpp"
#include "../headers/ConfigFile.hpp"

void    Accept(vector<pollfd>& fds, vector<t_client>& clients, size_t i)
{
   t_client client;
   pollfd   fd;
   int      clientFd;

    fcntl(fds[i].fd, F_SETFL, O_NONBLOCK);
    if ((clientFd = accept(fds[i].fd ,NULL ,NULL)) < 0)
        return ;

    fd.fd = clientFd;
    fd.events = POLLIN | POLLOUT;
    fds.push_back(fd);
    client.new_sock_fd = clientFd;
    client.body.clear();
    client.sessionID = generateRandomString(8);
    clients.push_back(client);
}
