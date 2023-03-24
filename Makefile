NAME = philo
CC = cc
CFLAGS = -Wextra -Werror -Wall
SRCS =	philosophers.c\
		philo_utils.c\
		philo_utils1.c\

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