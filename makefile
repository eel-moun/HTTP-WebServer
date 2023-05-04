CC = c++
FLAGS = -Wall -Werror -Wextra --std=c++98
SRC = src/ConfigFile.cpp src/Location.cpp src/Server.cpp src/parseConfig.cpp src/parseConfigUtils.cpp main.cpp
Headers = ./headers/ConfigFile.hpp ./headers/Location.hpp ./headers/parseConfig.hpp ./headers/Server.hpp
OBJ = $(SRC:.cpp=.o)
Name = websrv

all: $(Name)

$(Name):  $(OBJ) $(Headers)
	$(CC) $(FLAGS) $(OBJ) -o $(Name)

%.o : %.cpp $(Headers)
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf $(Name)

fclean: clean
	rm -rf $(OBJ);

re: clean $(Name)