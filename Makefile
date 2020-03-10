NAME = libftprintf.a
LIBFT = libft
INCDIR = includes
OBJDIR = obj
SRCDIR = src
CC = gcc
CCFLAGS := -Wall -Wextra -Werror

debug = yes

ifeq ($(debug), yes)
	CCFLAGS := -g $(CCFLAGS)
endif

SRC = ft_printf.c pf_strclone.c pf_itoa_base.c pf_utoa_base.c pf_simple_atoi.c\
   	  pf_percent_conv.c pf_csp_conv.c pf_diuox_conv.c pf_default_conv.c

OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

$(shell mkdir -p $(OBJDIR))


all: $(NAME)

$(NAME): $(OBJ)
	make -C $(LIBFT)
	cp $(LIBFT)/libft.a ./$(NAME)
	ar rcs $(NAME) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/libftprintf.h
	$(CC) $(CCFLAGS) -c -o $@ $< -I $(INCDIR)

.PHONY: all clean fclean re

clean:
	rm -rf $(OBJDIR)
	make -C $(LIBFT) clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT) fclean
re: fclean
	make all

