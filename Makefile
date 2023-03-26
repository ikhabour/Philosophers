NAME = philo
CC = cc
CFLAGS = -Wextra -Werror -Wall
SRCS =	philosophers.c\
		philo_utils.c\
		philo_utils1.c\
		philo_utils2.c

OBJECTS = ${SRCS:.c=.o}

all : $(NAME)

$(NAME) : $(OBJECTS)
	$(CC) $(CFLAGS) -pthread $(SRCS) -o $(NAME)
	@echo "Philo Created."

clean :
	rm -rf $(OBJECTS)
	@echo "Object files removed."

fclean : clean
	rm -rf $(NAME)
	@echo "Binary & Object files removed."

re : fclean all

.PHONY : all clean fclean re