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
	  pf_basic_conv.c\
	  pf_diuox_conv.c\
	  pf_double_conv.c\
	  pf_strchr_idx.c\
	  pf_simple_atoi.c\
	  pf_utoa_base.c\
	  pf_dtoa.c\
	  bignum/bignum.c\
	  math/ft_fabs.c\
	  math/ft_isinf.c\
	  math/ft_isnan.c\
	  math/ft_issub.c\
	  math/ft_trunc.c

OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

$(shell mkdir -p $(OBJDIR) $(OBJDIR)/bignum $(OBJDIR)/math)


all: $(NAME)

export debug
$(NAME): $(OBJ)
	make -C $(LIBFT)
	@echo "# packing $(NAME)"
	cp $(LIBFT)/libft.a ./$(NAME)
	ar rcs $(NAME) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c includes/libftprintf.h
	$(CC) $(CCFLAGS) -c -o $@ $< $(INCFLAGS)
$(OBJDIR)/pf_double_conv.o: includes/float.h
$(OBJDIR)/pf_dtoa.o: includes/bignum.h includes/float.h
$(OBJDIR)/bignum/%.o: includes/bignum.h
$(OBJDIR)/math/%.o: includes/float.h

.PHONY: all clean fclean re

clean:
	rm -rf $(OBJDIR)
	make -C $(LIBFT) clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT) fclean
re: fclean
	make all

