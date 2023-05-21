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