NAME = webserv
CC = c++
#CFLAGS = -Wall -Werror -Wextra -std=c++98 -g3 -fsanitize=address -Iinc
CFLAGS = -Wall -Werror -Wextra -std=c++98 -g3 -Iinc
#CFLAGS = -Wall -Werror -Wextra -g3 -Iinc
SRCS_DIR = srcs/
SRCS =	Request.cpp Response.cpp AutoIndex.cpp CGI_Handler.cpp \
		ListeningSocket.cpp main.cpp ClientSocket.cpp Logger.cpp \
		Webserv_machine.cpp ASocket.cpp Server.cpp Location.cpp ConfigParser.cpp utils.cpp

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
