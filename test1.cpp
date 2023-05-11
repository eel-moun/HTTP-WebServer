#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
using namespace std;
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

typedef struct Client
{
    int new_sock_fd;
    map<string, string> request;

}   t_client;

string lineToParse(string key, string buffer){
    int pos;
    string res;

    pos = 0;
    if (!key.compare("first"))
    {
        pos = buffer.find("\r\n");
        res = buffer.substr(0, pos);
        return res;
    }

    while (buffer.at(0) != '\r')
    {
        pos = buffer.find("\r\n");
        res = buffer.substr(0, pos);
        if (res.find(key) != string::npos)
            return res;
        buffer.erase(0, pos + 2);
    }
    return "";
}

void    parseRequest(t_client& client, string buffer)
{
    string key[8] = {"method", "path", "protocol", "host", "port", "media-type", "charset", "lenght"};
    string str;
    int i = 0;

    stringstream ss(lineToParse("first", buffer));
    while (getline(ss, str, ' '))
        client.request.insert(pair<string, string>(key[i++], str));

    stringstream ss1(lineToParse("host", buffer));
    if (getline(ss1, str, ':'))
    {
        if (str.compare("Host"))
            throw runtime_error("unwanted value");
        if (getline(ss1, str, ':'))
            client.request.insert(pair<string, string>(key[i++], str.substr(1)));
        if (getline(ss1, str, ':'))
            client.request.insert(pair<string, string>(key[i++], str));
    }

    stringstream ss2(lineToParse("content-type", buffer));
    if (getline(ss2, str, ' '))
    {
        if (str.compare("Content-Type:"))
            throw runtime_error("unwanted value");
        if (getline(ss2, str, ';'))
            client.request.insert(pair<string, string>(key[i++], str));
        if (getline(ss2, str, ';'))
            client.request.insert(pair<string, string>(key[i++], str.substr(1)));
    }
    
    str = lineToParse("content-length", buffer);
    if (str.compare("Content-Length"))
        throw runtime_error("unwanted value");
    client.request.insert(pair<string, string>(key[i++], str.substr(16)));

}

  

int main()
{
    t_client client;
    string key[8] = {"method", "path", "protocol", "host", "port", "media-type", "charset", "lenght"};
    char buffer[1024];
    int fd;

    bzero(buffer, 1024);
    fd = open("file.txt", O_RDWR);
    if (fd == -1)
        cout << "boo" << endl;
    read(fd, buffer, 1024);
    cout << "///////////////////////////" << endl;
    cout << buffer << endl;
    cout << "///////////////////////////" << endl;
    parseRequest(client, buffer);
    for (int i = 0; i > 7; i++)
        cout << client.request[key[i]] << endl;
}

// int main(int argc, char const* argv[])
// {
//     int status, valread, client_fd;
//     struct sockaddr_in serv_addr;
//     char* hello = "Hello from client";
//     char buffer[1024] = { 0 };
//     if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         printf("\n Socket creation error \n");
//         return -1;
//     }
  
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);
  
//     // Convert IPv4 and IPv6 addresses from text to binary
//     // form
//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
//         <= 0) {
//         printf(
//             "\nInvalid address/ Address not supported \n");
//         return -1;
//     }
  
//     if ((status
//          = connect(client_fd, (struct sockaddr*)&serv_addr,
//                    sizeof(serv_addr)))
//         < 0) {
//         printf("\nConnection Failed \n");
//         return -1;
//     }
//     send(client_fd, hello, strlen(hello), 0);
//     printf("Hello message sent\n");
//     valread = read(client_fd, buffer, 1024);
//     printf("%s\n", buffer);
  
//     // closing the connected socket
//     close(client_fd);
//     return 0;
// }