#include "headers/parseConfig.hpp"
#include "headers/ConfigFile.hpp"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


void generate_servers(ConfigFile& config)
{
    for(size_t i = 0; i < config.getSize(); i++)
    {
        cout << "test" << endl;
        config.getServer(i)->openServer();
    }

}