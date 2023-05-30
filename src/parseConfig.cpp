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
    array<string, 14> keys = {"host", "default_error", "max_size", "server_name", "listen", "server", "location", "path", "index", "root", "allowed_method", "autoindex", "upload_dir", "return"};
    int i = 0;
    int ser_size = config.getSize();
    size_t loc_size;

    if (ser_size > 0)
        loc_size = config.getServer(ser_size - 1)->getSize();

    while (i <= 13 && key.compare(keys[i]))
        i++;

    if (i <= 3)
        config.getServer(ser_size - 1)->setValue(key, value);

    else switch (i)
    {
        {
            case 4:
                config.getServer(ser_size - 1)->set_listens(value);
                break;
            case 5:
                config.setServer();
                break;
            case 6:
                config.getServer(ser_size - 1)->setLocation();
                break;
            case 7:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                else if (value.substr(value.size() - 1) == "/" && value.size() != 1)
                    throw invalid_argument("can't be '/' in end of path");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setPath(value);
                break;
            case 8:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setIndex(value);
                break;
            case 9:
                if (loc_size == 0)
                {
                    config.getServer(ser_size - 1)->setValue(key, value);
                    break;
                }
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setRoot(value);
                break;
            case 10:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setAllowedMethod(value);
                break;
            case 11:
                if (loc_size == 0)
                {
                    config.getServer(ser_size - 1)->setValue(key, value);
                    break;
                }
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setAutoIndex(value);
                break;
            case 12:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->set_upload_dir(value);
                break;
            case 13:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->set_return(value);
                break;
            default:
                cout << key << endl;
                throw invalid_argument("Key not supported");
        }
    }
}
