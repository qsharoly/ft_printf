NAME = libftprintf.a
INCDIR = includes 
OBJDIR = obj
SRCDIR = src
LIBFT = libft
LFT_INCDIR = $(LIBFT)/includes
CC = gcc
CCFLAGS = -Wall -Wextra -Werror
INCFLAGS = -I$(INCDIR) -I$(LFT_INCDIR)

debug = yes

ifeq ($(debug), yes)
	CCFLAGS += -g
endif

SRC = ft_printf.c\
	  pf_parse_specifier.c\
	  pf_arg_to_string.c\
	  pf_basic_conv.c\
	  pf_diuox_conv.c\
	  pf_double_conv.c\
	  pf_strchr_idx.c\
	  pf_strclone.c\
	  pf_simple_atoi.c\
	  pf_itoa_dec.c\
	  pf_utoa_dec.c\
	  pf_utoa_oct.c\
	  pf_utoa_hex.c\
	  pf_dtoa.c\
	  pf_ldtoa.c\
	  bignum/bignum.c

OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

$(shell mkdir -p $(OBJDIR); mkdir -p $(OBJDIR)/bignum)


all: $(NAME)

$(NAME): $(OBJ)
	make -C $(LIBFT)
	cp $(LIBFT)/libft.a ./$(NAME)
	ar rcs $(NAME) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c includes/libftprintf.h
	$(CC) $(CCFLAGS) -c -o $@ $< $(INCFLAGS)

.PHONY: all clean fclean re

clean:
	rm -rf $(OBJDIR)
	make -C $(LIBFT) clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT) fclean
re: fclean
	make all

