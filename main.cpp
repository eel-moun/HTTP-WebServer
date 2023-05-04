#include "./headers/parseConfig.hpp"

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
	parseConfig(conf_file, config);
}

string parseConfig(ifstream& conf_file, ConfigFile& config)
{
    string line;

    while (getline(conf_file, line, '\n'))
    {

    }
}

// hello mother fucker

int main(int ac, char **av)
{
    ConfigFile  config;
    checkConfigFile(ac, av, config);

    return (0);
}
