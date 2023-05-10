#include "./headers/parseConfig.hpp"
#include "headers/ServerUtils.hpp"

void	checkConfigFile(int ac, char **av, ConfigFile& config)
{
    ifstream &conf_file = openFileStream(ac ,av);
	parseConfig(conf_file, config);
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

    return (0);
}
