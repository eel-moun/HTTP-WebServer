#include "../headers/ConfigFile.hpp"

string lineToParse(string key, string buffer){
    int pos;
    string res;

    pos = buffer.find("\r\n");
    if (!key.compare("first"))
    {
        res = buffer.substr(0,pos);
        return res;
    }
    buffer.erase(0,pos+2);
    while (buffer.at(0) != '\r')
    {
        pos = buffer.find("\r\n");
        res = buffer.substr(0, pos);
        if(res.find(key) != string::npos)
            return res;
        buffer.erase(0, pos + 2);
    }
    return "";
}

void chunkedToNormal(t_client& client, string buffer)
{
    char buffer1[1024];
    string tmp;
    int r;
    size_t len;

    r = 0;
    buffer.erase(0, buffer.find("\r\n\r\n") + 4);
    buffer.erase(0, buffer.find("\r\n\r\n") + 4);
    while (r != -1 && buffer.find("0\r\n\r\n") != 0 && len != 0)
    {
        if(buffer.find("\r\n") == 0)
            buffer.erase(0,2);
        bzero(buffer1,1024);
        r = read(client.new_sock_fd, buffer1, 1023);
        if(r >= 0)
            buffer += string(buffer1,r);
        tmp = buffer.substr(0, buffer.find("\r\n"));
        buffer.erase(0, buffer.find("\r\n") + 2);

        if(tmp.size())
        {
            len = std::stol(tmp, 0, 16);
            tmp.clear();
        }
        while(buffer.size() < len)
        {
            bzero(buffer1,1024);
            r = read(client.new_sock_fd, buffer1, 1023);
            if(r >= 0)
                buffer += string(buffer1,r);
        }
        cout <<  buffer.size()<< endl;
        while(tmp.size() < len)
        {
            tmp += buffer.substr(0, len);
            buffer.erase(0, len + 2);
            cout << tmp.size() << endl;
            cout <<  buffer.size()<< endl;
        }
        cout << buffer.size() << endl;
        cout << tmp.size() << endl;
        cout << tmp.substr(0,10) << endl;
        cout << tmp.substr(tmp.size() -10) << endl;
        client.body += tmp;
    }
}

size_t getLocationIndex(string req_path, Server server)
{
    string  location_path;
    size_t  size = 0;
    size_t  j;


    req_path.substr(location_path.size() +1);
    for (size_t i = 0; i < server.getSize(); i++)
    {
        location_path =server.getLocation(i)->getPath();
        if (req_path.find(location_path) == 0)
        {
            if (size < location_path.size())
            {
                size = location_path.size();
                j = i;
            }
        }
    }
    return j;
}

void normalBody(t_client& client,string buffer)
{
    int r = 0;
    char buffer1[1024];
    size_t lenght = strtol(client.request["lenght"].c_str(),0,10);
    size_t lenght2 = 0;

    if((size_t)strtol(client.request["max_size"].c_str(),0,10) < lenght){
        GenerateResponse("", "", 400, client);
    }
    if(client.body.size() == 0)
    {
        buffer.erase(0, buffer.find("\r\n\r\n") + 4);
        lenght2 = buffer.find("\r\n\r\n") + 4;
        buffer.erase(0, buffer.find("\r\n\r\n") + 4);
    }
    client.body += buffer;
    while((client.body.size()+ lenght2) < lenght)
    {
        bzero(buffer1,1024);
        r = read(client.new_sock_fd, buffer1, 1023);
        if (r != -1)
            client.body += string(buffer1,r);
    }
}

string generateRandomString(int length) {
    string randomString;
    srand(time(0));

    for (int i = 0; i < length; ++i) {
        randomString += static_cast<char>('a' + rand() % 26);
    }
    return randomString;
}

void fillBody(t_client& client,string buffer)
{
    if(client.request["lenght"].size())
        normalBody(client, buffer);
    else if(!client.request["Transfer-Encoding"].compare("chunked"))
        chunkedToNormal(client, buffer);
}