
##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Makefile
##

rwildc = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildc,$d/,$2))

SOURCEDIR = ../../src/server
SOURCEDIR_2 = ./

SRC_TMP = $(call rwildc,$(SOURCEDIR),*.c) $(call rwildc,$(SOURCEDIR_2),*.c)
SRC = $(filter-out ../../src/server/main.c, $(SRC_TMP))

CC = gcc

NAME = tests

CFLAGS = -I ../../include/server -lm --coverage -lcriterion -g

all: $(NAME)

$(NAME): fclean
	$(CC) -o $(NAME) $(SRC) $(CFLAGS) $(INCLUDE) $(LIB)

clean:
	rm -f $(OBJ) || true
	find .. -name "*~" -delete || true
	find .. -name "*.gcno" -delete || true
	find .. -name "*.gcda" -delete || true

fclean:    clean
	rm -f $(NAME)

re:        fclean all

run: re
	./$(NAME) ; rm -f $(NAME)
