##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Makefile
##

rwildc = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildc,$d/,$2))


SOURCEDIR_SERVER = src/server
SRC_SERVER = $(call rwildc,$(SOURCEDIR_SERVER),*.c)
OBJ_SERVER = $(SRC_SERVER:.c=.o)
SERVER_NAME = zappy_server
CC = gcc
CFLAGS = -Wall -Wextra -I ./include/server -g

SOURCEDIR_GUI = src/gui
SRC_GUI = $(call rwildc,$(SOURCEDIR_GUI),*.cpp)
OBJ_GUI = $(SRC_GUI:.cpp=.o)
GUI_NAME = zappy_gui
CXX = g++
CXXFLAGS = -Wall -Wextra -I ./include/gui -std=c++20

all: $(SERVER_NAME) $(GUI_NAME)

server: $(SERVER_NAME)
gui: $(GUI_NAME)

$(SERVER_NAME): $(OBJ_SERVER)
	$(CC) -o $(SERVER_NAME) $(OBJ_SERVER) $(CFLAGS)

$(GUI_NAME): $(OBJ_GUI)
	$(CXX) -o $(GUI_NAME) $(OBJ_GUI) $(CXXFLAGS)

tests_run:
	echo "No tests for this project"
# 	make -C tests
# 	cd tests && ./tests ; rm tests

clean:
	rm -f $(OBJ_SERVER) $(OBJ_GUI)

fclean:    clean
	rm -f $(SERVER_NAME) $(GUI_NAME)

re:        fclean all
