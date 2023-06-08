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
    client.total_read = 0;
    client.header_removed = false;
    client.finished_reading = false;
    client.chunked_len_set = false;
    client.chunked_len = 0;
    client.w = 0;
    client.timeout = time(NULL);
    clients.push_back(client);
}


void disconect(vector<pollfd>& fds, vector<t_client>& clients, size_t i, size_t socket_num)
{
    cout << "client " << fds[i].fd << " disconnected" << endl;
    close(fds[i].fd);
    fds.erase(fds.begin() + i);
    clients.erase(clients.begin() + (i - socket_num));
}
