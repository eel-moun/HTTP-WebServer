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

void parseConfig(ifstream& conf_file, ConfigFile& config)
{
    string line;

	/* This line of code is skipping over any empty lines at the beginning of the configuration file. It
	reads each line of the file using `getline()` and checks if the size of the line is zero (i.e., it
	is empty). If the line is empty, it continues to the next line until it finds a non-empty line. */
	while (getline(conf_file, line, '\n') && line.size() == 0)
		continue;

	/* This line of code is checking if the first non-empty line of the configuration file starts with the
	string "server " followed by an opening curly brace '{'. If it does not, or if the line is not
	exactly 7 characters long, it throws an invalid_argument exception with the message "config file
	syntax error". This is to ensure that the configuration file is properly formatted and starts with
	a server block. */
	if (getline(conf_file, line, '{') || line.substr(0, 7) != "server " || line.size() != 7)
		throw invalid_argument("config file syntax error");

    /* This code block is responsible for parsing the configuration file and extracting information
	about server and location settings. It reads each line of the file using `getline()` and removes
	any leading or trailing spaces using the `rm_spaces()` function. It then checks if the line
	starts with "location {" or "location" or "}" or "server" and calls the appropriate function
	(`test()` or `parseConfig()`) based on the line content. If the line starts with "location {",
	it enters a nested while loop that reads each line until it finds a line that contains only "}"
	and calls the `test()` function with the line content and "location" as arguments. If the line
	starts with "location", it throws an invalid_argument exception with the message "config file
	syntax error". If the line contains only "}", it calls the `parseConfig()` function recursively
	to parse the next server block. If the line starts with "server", it calls the `test()` function
	with the line content and "server" as arguments. */
    test("server", config);
	while (getline(conf_file, line, '\n'))
    {
		rm_spaces(line);
		if (line.substr(0, 10) == "location {")
		{
			if (line.size() != 10)
				throw invalid_argument("config file syntax error");
            test("location", config);
			while (getline(conf_file, line, '\n'))
			{
				rm_spaces(line);
				if (line.substr(0, 1) == "}" && line.size() == 1)
					break;
				test(line, config);
			}
			continue;
		}
		else if (line.substr(0, 8) == "location")
			throw invalid_argument("config file syntax error");
		if (line.substr(0, 1) == "}" && line.size() == 1)
		{
			parseConfig(conf_file, config);
			return ;
		}
		test(line, config);
    }
}

int main(int ac, char **av)
{
    ConfigFile  config;
    checkConfigFile(ac, av, config);

    return (0);
}
