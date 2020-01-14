NAME = test.out
WFLAGS = -Wall -Wextra -Werror
SRC = ft_printf.c ft_itoa_base.c ft_strlen.c

all:
	gcc $(WFLAGS) $(SRC) -o $(NAME)
