#ifndef PARSECONFIG_HPP
#define PARSECONFIG_HPP

#include "ConfigFile.hpp"
#include <string>
#include <fstream>
#include <array>
#include "Client.hpp"

using namespace std;
#define DEFAULT_PATH "default/nginx.conf"

void        rmSpaces(string& line);
ifstream&   openFileStream(int ac ,char **av);
void        checkConfigFile(int ac, char **av, ConfigFile& config);
int         keyValue(string line, ConfigFile& config);
void        fillConfigFile(string key, string value, ConfigFile& config);
void        parseConfig(ifstream& conf_file, ConfigFile& config);
void	    checkDefault(ConfigFile& config);

#endif