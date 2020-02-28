NAME = libftprintf.a
CC = gcc
DBGFLAG = -g
WFLAGS = -Wall -Wextra -Werror
OBJDIR = obj

SRC = ft_printf.c ft_itoa_base.c ft_itoa_base_unsigned.c ft_simple_atoi.c ft_strlen.c ft_strupper.c
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

$(shell mkdir -p $(OBJDIR))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

$(OBJDIR)/%.o: %.c
	$(CC) $(DBGFLAG) $(WFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR)
fclean: clean
	rm -f $(NAME)
re: fclean
	make all

