NAME = webserv
CC = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g3 -Iinc
SRCS_DIR = srcs/
SRCS = main.cpp Webserv_machine.cpp Socket.cpp Server.cpp Location.cpp
OBJ/OBJECTS		=	$(patsubst $(SRCS_DIR)%.cpp, obj/%.o, $(SRCS))
SRCS	:= $(foreach file,$(SRCS),$(SRCS_DIR)$(file))

all: $(NAME)
	
obj/%.o: $(SRCS_DIR)%.cpp Makefile | obj
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ/OBJECTS)
	$(CC) -o $(NAME) $^ $(CFLAGS)

obj:
	mkdir obj

clean:
	rm -rf obj
	rm -f .*.swp

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
