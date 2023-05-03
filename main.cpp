#include "iostream"
#include <string>
#include <fstream>
#include <exception>
#include "headers/ConfigFile.hpp"
#include <array>

using namespace std;
#define DEFAULT_PATH "./config/default"

ifstream& openFileStream(int ac ,char **av)
{
    if (ac > 2)
        throw invalid_argument("arguments  > 2");
    else if (ac == 2)
    {
        ifstream conf_file(static_cast<string>(av[1]));
        if (!conf_file)
            return (cout << "error in opening config file\n", conf_file);
        return conf_file;
    }
	else
	{
		ifstream conf_file(static_cast<string>(DEFAULT_PATH));
		if (!conf_file)
            return (cout << "error in opening default config file\n", conf_file);
        return(conf_file);
	}
}

int	checkConfigFile(int ac, char **av, ConfigFile& config)
{
    ifstream &conf_file = openFileStream(ac ,av);
	//parseConfig(conf_file, config);
}

void removeWS(string& line)
{
    while((line[0] == ' '|| line[0] == '\t' || line[0] == '\n' || line[0] == '\r' || line[0] == '\v' || line[0] == '\f') && line.size())
        line.erase(0,1);
}
int keyValue(string line, ConfigFile& config)
{
    size_t delimiterPos;
    string key;
    string value;

    removeWS(line);
    if(!line.size())
        return(0);
    delimiterPos = line.find(" ");
    key = line.substr(0 , delimiterPos);
    value = line.substr(delimiterPos + 1);
    fillConfigFile(key, value, config);
}

void fillConfigFile(string key, string value, ConfigFile& config)
{
    array<string, 13> keys{"listen", "host", "server_name","error_page400", "error_page402", "error_page403", "error_page404", "error_page500", "error_page501", "server", "location", "path", "index"};
    int i = 0;
    int index = config.getSize();
    int index2 = config.getServer(index - 1).getSize();

    while(!key.compare(keys[i]))
        i++;
    switch (i)
    {
        case 1:
            config.getServer(index - 1).setListen(value);
            break;
        case 2:
            config.getServer(index - 1).setHost(value);
            break;
        case 3:
            config.getServer(index - 1).setServerName(value);
            break;
        case 4:
            config.getServer(index - 1).setErrorPage(value, 400);
            break;
        case 5:
            config.getServer(index - 1).setErrorPage(value, 402);
            break;
        case 6:
            config.getServer(index - 1).setErrorPage(value, 403);
            break;
        case 7:
            config.getServer(index - 1).setErrorPage(value, 404);
            break;
        case 8:
            config.getServer(index - 1).setErrorPage(value, 500);
            break;
        case 9:
            config.getServer(index - 1).setErrorPage(value, 501);
            break;
        case 10:
            config.setServer();
            break;
        case 11:
            config.getServer(index - 1).setLocation();
            break;
        case 12:
            config.getServer(index - 1).getLocation(index2 - 1).setPath(value);
            break;
        case 13:
            config.getServer(index - 1).getLocation(index2 - 1).setIndex(value);
            break;
        default:
            throw invalid_argument("Key not supported");
    }
    
}

int main(int ac, char **av)
{
    ConfigFile  config;
    checkConfigFile(ac, av, config);

    return (0);
}
