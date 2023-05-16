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
        res = buffer.substr(0,pos);
        if(res.find(key) != string::npos)
            return res;
        buffer.erase(0,pos+2);
    }
    return "";
}

string chunkedToNormal(string buffer)
{
    string tmp;
    string res;
    size_t len;

    len = 1;
    buffer.erase(0, buffer.find("\r\n\r\n") + 4);
    while(buffer.find("0\r\n\r\n"))
    {
        if(buffer.find("\r\n") == 0)
            buffer.erase(0,buffer.find("\r\n") + 2);
        
        len = stoi(buffer.substr(0, buffer.find("\r\n")), 0, 16);
        buffer.erase(0,buffer.find("\r\n") + 2);

        tmp = buffer.substr(0, buffer.find("\r\n"));
        if(tmp.size() == len)
        {
            res += tmp;
            buffer.erase(0,buffer.find("\r\n") + 2);
        }
        else{
            throw runtime_error("wrong size in chunked request");
        }
    }
    return (res);
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

string generateRandomString(int length) {
    string randomString;
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(97, 122);

    for (int i = 0; i < length; ++i) {
        randomString += static_cast<char>(distribution(generator));
    }

    return randomString;
}