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
    while (buffer.find("\r\n") != string::npos)
    {
        pos = buffer.find("\r\n");
        res = buffer.substr(0, pos);
        if(res.find(key) != string::npos)
            return res;
        buffer.erase(0, pos + 2);
    }
    return "";
}

void chunkedToNormal(t_client& client, string buffer, Server server)
{
    string tmp;
    (void) server;
    if(!client.header_removed)
    {
        buffer.erase(0, buffer.find("\r\n\r\n") + 4);
        if(client.request["boundary"].size())
            buffer.erase(0, buffer.find("\r\n\r\n") + 4);
        client.header_removed = true;
    }
    client.buffer += buffer;
    while (client.buffer.size())
    {
        if(client.buffer.find("\r\n") == 0)
            client.buffer.erase(0,2);
        if(!client.chunked_len_set && client.buffer.size())
        {
            if(client.buffer.find("\r\n") != string::npos)
            {
                tmp = client.buffer.substr(0, client.buffer.find("\r\n"));
                client.chunked_len = std::strtol(tmp.c_str(), 0, 16);
                client.chunked_len_set = true;
                client.total_read = 0;
                client.buffer.erase(0, client.buffer.find("\r\n") + 2);
            }
            else
                return ;
            if(client.chunked_len == 0)
            {
                client.finished_reading = true;
                return ;
            }
        }
        if(client.total_read < client.chunked_len && client.chunked_len_set)
        {
            if(client.buffer.size() < (client.chunked_len - client.total_read))
            {
                client.body += client.buffer;
                client.total_read += client.buffer.size();
                client.buffer.clear();
            }
            else{
                if(client.buffer.find("\r\n") != string::npos)
                {
                    client.body += client.buffer.substr(0, (client.chunked_len - client.total_read));
                    client.chunked_len_set = false;
                    client.buffer.erase(0, (client.chunked_len - client.total_read) + 2);
                    client.total_read += (client.chunked_len - client.total_read);
                }
                else
                    return ;
            }
        }
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

void normalBody(t_client& client, string buffer, Server server)
{
    size_t length = strtol(client.request["length"].c_str(),0,10);

    if(!client.header_removed)
    {
        buffer.erase(0, buffer.find("\r\n\r\n") + 4);
        client.header_removed = true;
        if(client.request["boundary"].size())
        {
            client.total_read += buffer.find("\r\n\r\n") + 4;
            buffer.erase(0, buffer.find("\r\n\r\n") + 4);
        }
    }
    client.total_read += buffer.size();
    client.body += buffer;
    if(client.total_read > length)
        GenerateResponse(getRightContent(open(server.getValue("400_error").c_str(), O_RDONLY)), (string&)"text/html", 400, client, "");
    else if(client.total_read == length)
        client.finished_reading = true;
}

string generateRandomString(int length)
{
    string randomString;
    srand(time(0));

    for (int i = 0; i < length; ++i) {
        randomString += static_cast<char>('a' + rand() % 26);
    }
    return randomString;
}

void fillBody(t_client& client, string buffer, Server server)
{
    if(client.request["length"].size())
        normalBody(client, buffer, server);
    else if(!client.request["Transfer-Encoding"].compare("chunked"))
        chunkedToNormal(client, buffer, server);
    else
        client.finished_reading = true;
}