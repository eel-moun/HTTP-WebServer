#include "./headers/parseConfig.hpp"

using namespace std;

int main(int ac, char **av)
{
    ConfigFile  config;
    checkConfigFile(ac, av, config);

    return (0);
}
