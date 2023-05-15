#include "../headers/ConfigFile.hpp"

string lineToParse(string key, string buffer){
    int pos;
    string res;

    pos = buffer.find("\r\n");
    cout << "<----------------->" << endl;
    cout << pos << endl;
    cout << "<----------------->" << endl;
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