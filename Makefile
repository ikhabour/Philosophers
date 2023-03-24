NAME = philo
CC = cc
CFLAGS = -Wextra -Werror -Wall
SRCS =	philosophers.c\
		ft_atoi.c\

OBJECTS = ${SRCS:.c=.o}

all : $(NAME)

$(NAME) : $(OBJECTS)
	$(CC) $(CFLAGS) -pthread $(SRCS) -o $(NAME)

clean :
	rm -rf $(OBJECTS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re