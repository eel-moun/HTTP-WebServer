#include "iostream"
#include <string>
#include <fstream>
#include <exception>
#include "headers/ConfigFile.hpp"

using namespace std;
#define DEFAULT_PATH "./config/default"

ifstream& openFileStream(int ac ,char **av)
{
    if (ac > 2)
        throw invalid_argument("arguments > 2");
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
	// here we gonna make the parse gerer bzaf dyal servers
	parseConfig(conf_file, config);
}

void	test(string &test, string code)
{}

/**
 * The function parses a configuration file and extracts information about server and location
 * settings.
 */
ifstream& parseConfig(ifstream& conf_file, ConfigFile& config)
{
    string line;

	if (!getline(conf_file, line, '{') || line.substr(0, 7) != "server " || line.size() != 7)
		throw invalid_argument("config file syntax error");

    while (getline(conf_file, line, '\n'))
    {
		if (line.substr(0, 10) == "location {")
		{
			if (line.size() != 10)
				throw invalid_argument("config file syntax error");
			while (getline(conf_file, line, '\n'))
			{
				if (line.find("}") != string::npos)
					break;
				test(line, "location");
			}
			continue;
		}
		test(line, "server");
    }
}

int main(int ac, char **av)
{
    ConfigFile  config;
    checkConfigFile(ac, av, config);

    return (0);
}
