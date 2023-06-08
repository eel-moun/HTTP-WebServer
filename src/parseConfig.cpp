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
    array<string, 23> keys = {"host", "default_error",  "301_error","400_error", "404_error", "405_error", "408_error", "500_error", "501_error", "max_size", "server_name", "listen", "server", "location", "path", "index", "root", "allowed_method", "autoindex", "upload_dir", "return", "cgi_ext", "cgi_path"};
    int i = 0;
    int ser_size = config.getSize();
    size_t loc_size;

    if (ser_size > 0)
        loc_size = config.getServer(ser_size - 1)->getSize();

    while (i <= 22 && key.compare(keys[i]))
        i++;

    if (i <= 10)
        config.getServer(ser_size - 1)->setValue(key, value);

    else switch (i)
    {
        {
            case 11:
                config.getServer(ser_size - 1)->set_listens(value);
                break;
            case 12:
                config.setServer();
                break;
            case 13:
                config.getServer(ser_size - 1)->setLocation();
                break;
            case 14:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                else if (value.substr(value.size() - 1) == "/" && value.size() != 1)
                    throw invalid_argument("can't be '/' in end of path");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setPath(value);
                break;
            case 15:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setIndex(value);
                break;
            case 16:
                if (loc_size == 0)
                {
                    config.getServer(ser_size - 1)->setValue(key, value);
                    break;
                }
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setRoot(value);
                break;
            case 17:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setAllowedMethod(value);
                break;
            case 18:
                if (loc_size == 0)
                {
                    config.getServer(ser_size - 1)->setValue(key, value);
                    break;
                }
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->setAutoIndex(value);
                break;
            case 19:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->set_upload_dir(value);
                break;
            case 20:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->set_return(value);
                break;
            case 21:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->set_cgi_ext(value);
                break;
            case 22:
                if (loc_size == 0)
                    throw invalid_argument("declare a location");
                config.getServer(ser_size - 1)->getLocation(loc_size - 1)->set_cgi_path(value);
                break;
            default:
                throw invalid_argument("Key not supported");
        }
    }
}
