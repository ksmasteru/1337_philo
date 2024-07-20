NAME = philo
CC = cc
CFLAGS = -g -fsanitize=address
SRCS = main.c routines.c
OBJS = $(SRCS:.c=.o)


all : $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean :
	@rm -f $(OBJS)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean	
