#include "iostream"
#include "fcntl.h"
using namespace std;

int	checkConfigFile(int ac, char **av)
{
	int fd;

    if (ac > 2)
        return (cout << "enter the correct number of argument\n", -1);
    else if (ac == 2)
    {
        fd = open(av[1], O_RDONLY);
        if (fd == -1)
            return (cout << "error in opening config file\n", -1);
    }
	else
	{
		fd = open("default/nginx.conf", O_RDONLY);
		if (fd == -1)
            return (cout << "error in opening default config file\n", -1);
	}
	return (fd);
}

int main(int ac, char **av)
{
    int fd;

    if (checkConfigFile(ac, av) == -1)
		return (-1);

	
}
