#include "../headers/parseConfig.hpp"


int keyValue(string line, ConfigFile& config)
{
    size_t delimiterPos;
    string key;
    string value;

    if(!line.size())
        return(0);
    delimiterPos = line.find(" ");
    key = line.substr(0 , delimiterPos);
    value = line.substr(delimiterPos + 1);
    rmSpaces(value);
    fillConfigFile(key, value, config);
    return (0);
}

void fillConfigFile(string key, string value, ConfigFile& config)
{
    array<string, 14> keys = {"listen", "host", "server_name","error_page400", "error_page402", "error_page403", "error_page404", "error_page500", "error_page501", "server", "location", "path", "index", "try_files"};
    int i = 0;
    int index = config.getSize();
    size_t index2;

    if (index > 0)
        index2 = config.getServer(index - 1)->getSize();

    while (i <= 13 && key.compare(keys[i]))
        i++;

    if (i < 9)
        config.getServer(index - 1)->setValue(key, value);
    else {
        switch (i)
        {
            case 9:
                config.setServer();
                break;
            case 10:
                config.getServer(index - 1)->setLocation();
                break;
            case 11:
                if(index2 == 0)
                    throw invalid_argument("declare a location");
                config.getServer(index - 1)->getLocation(index2 - 1)->setPath(value);
                break;
            case 12:
                if(index2 == 0)
                {
                    config.getServer(index - 1)->setValue(key, value);
                    break;
                }
                config.getServer(index - 1)->getLocation(index2 - 1)->setIndex(value);
                break;
            case 13:
                if(index2 == 0)
                    throw invalid_argument("declare a location");
                config.getServer(index - 1)->getLocation(index2 - 1)->setFile(value);
                break;
            default:
                cout << key << endl;
                throw invalid_argument("Key not supported");
        }
    }
}
