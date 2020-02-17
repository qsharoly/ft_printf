NAME = libftprintf.a
WFLAGS = -Wall -Wextra -Werror
OBJDIR = obj

SRC = ft_printf.c ft_itoa_base.c ft_strlen.c ft_strupper.c
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

$(shell mkdir -p $(OBJDIR))

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

$(OBJDIR)/%.o: %.c
	gcc $(WFLAGS) -c -o $@ $<
