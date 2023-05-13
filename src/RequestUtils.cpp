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

string chunkedToNormal(string buffer ,string res)
{
    string chunkedBody;
    string tmp;
    int len;

    len = 1;
    chunkedBody = buffer.substr(0, buffer.find("/r/n/r/n") + 4);
    while(len != 0)
    {
        len = stoi(buffer.substr(0, buffer.find("/r/n")));
        buffer.erase(0,buffer.find("/r/n") + 2);
        tmp = substr(0, buffer.find("/r/n"));
        if(tmp.size() == len)
        {
            res.push_back(tmp);
            buffer.erase(0,buffer.find("/r/n") + 2);
        }
        else{
            cout << "wrong len in chunkedToNormal" << endl;
            //error
        }
    }
    return (res);
}