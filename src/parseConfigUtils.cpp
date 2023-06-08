#include "../headers/parseConfig.hpp"

void rmSpaces(string& line)
{
    while((line[0] == ' '|| line[0] == '\t' || line[0] == '\n' || line[0] == '\r' || line[0] == '\v' || line[0] == '\f') && line.size())
        line.erase(0,1);
}

ifstream& openFileStream(int ac ,char **av)
{
    if (ac > 2)
        throw invalid_argument("arguments  > 2");
    else if (ac == 2)
    {
        ifstream* conf_file = new ifstream(static_cast<string>(av[1]));
        if (!*conf_file)
        {
            //delete conf_file;
            throw runtime_error("error in opening config file");
        }
        return *conf_file;
    }
	else
	{
		ifstream* conf_file = new ifstream(static_cast<string>(DEFAULT_PATH));
		if (!*conf_file)
        {
            //delete conf_file;
            throw runtime_error("error in opening config file");
        }
        return (*conf_file);
	}
}
