NAME = philo
CC = cc
CFLAGS = -g3 -fsanitize=thread -Wall #-Werror -Wextra
SRCS = main.c routines.c ft_atoi.c actions.c monitors.c parsing.c mixed_actions.c ft_split.c
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
