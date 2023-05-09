#include "./headers/parseConfig.hpp"
#include <sys/socket.h>

int main(){
    int server_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        perror("socket not created");
        return 0;
    }
    if(setsockopt())

}