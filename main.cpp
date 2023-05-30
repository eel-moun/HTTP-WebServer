#include "./headers/parseConfig.hpp"
#include "headers/ServerUtils.hpp"

void	checkDefault(ConfigFile& config)
{
	int	count = 0;

	for (size_t i = 0; i < config.getSize(); i++)
	{
		for (size_t j = 0; j < config.getServer(i)->getSize(); j++)
		{
			if (!config.getServer(i)->getLocation(j)->getPath().compare("/"))
				count++;
		}
		if (count != 1)
			throw invalid_argument("config file syntax error in default location");
		count = 0;
	}
}

void	checkConfigFile(int ac, char **av, ConfigFile& config)
{
    ifstream &conf_file = openFileStream(ac ,av);
	parseConfig(conf_file, config);
	checkDefault(config);
}

void parseConfig(ifstream& conf_file, ConfigFile& config)
{
    string line;

	while (getline(conf_file, line, '\n') && line.size() == 0)
		continue;

	if (!line.size())
		return ;

	if (line.substr(0, 7) != "server " || line.size() != 8)
	{
		cout << line << endl;
		throw invalid_argument("config file syntax error1");
	}

	keyValue("server", config);
	while (getline(conf_file, line, '\n'))
    {
		rmSpaces(line);
		if (line.substr(0, 10) == "location {")
		{
			if (line.size() != 10)
				throw invalid_argument("config file syntax error2");
			keyValue("location", config);
			while (getline(conf_file, line, '\n'))
			{
				rmSpaces(line);
				if (line.substr(0, 1) == "}" && line.size() == 1)
					break;
				keyValue(line, config);
			}
			continue;
		}
		else if (line.substr(0, 8) == "location")
			throw invalid_argument("config file syntax error3");

		if (line.substr(0, 1) == "}" && line.size() == 1)
		{
			parseConfig(conf_file, config);
			return ;
		}
		keyValue(line, config);
    }
}

int main(int ac, char **av)
{
    ConfigFile  config;
    checkConfigFile(ac, av, config);
	config.run_servers();

    return (0);
}


/*
        if (buffer.find("\r\n") == string::npos)
        {
            tmp = buffer.substr(0);
            buffer.erase(0, buffer.size());
            while(buffer.find("\r\n") == string::npos && buffer.size() < len)
            {   
                bzero(buffer1,1024);
                r = read(client.new_sock_fd, buffer1, 1023);
                if(r >= 0)
                    buffer += string(buffer1,r);
            }
            tmp += buffer.substr(0, buffer.find("\r\n"));
            buffer.erase(0, buffer.find("\r\n") + 2);
        }else
        {
            tmp = buffer.substr(0, buffer.find("\r\n"));
            buffer.erase(0, buffer.find("\r\n") + 2);
        }
        cout << tmp.size() << endl;
        cout << "<--------------->" << endl;

        if(len == -1 && tmp.size())
        {
            len = std::stol(tmp, 0, 16);
        }
        else{
            client.body += tmp;
            max_lenght += len;
            len = -1;
        }
    }*/