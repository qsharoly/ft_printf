NAME = libftprintf.a
INCDIR = includes 
OBJDIR = obj
SRCDIR = src
LIBFT = libft
LFT_INCDIR = $(LIBFT)/includes
CC = gcc
CCFLAGS = -Wall -Wextra -Werror
INCFLAGS = -I$(INCDIR) -I$(LFT_INCDIR)

#Disable union ABI warning on linux gcc
UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CCFLAGS += -Wno-psabi
endif

debug = yes
ifeq ($(debug), yes)
	CCFLAGS += -g
endif

SRC = ft_printf.c\
	  pf_stream.c\
	  pf_stream_helpers.c\
	  pf_specifier_parse.c\
	  pf_arg_get.c\
	  pf_conv_basic.c\
	  pf_conv_putnbr.c\
	  pf_conv_diuox.c\
	  pf_conv_floating.c\
	  pf_strget_index.c\
	  pf_simple_atoi.c\
	  pf_utoa_base.c\
	  pf_dtoa.c\
	  pf_dtoa_quick.c\
	  bignum/big_add.c\
	  bignum/big_carry.c\
	  bignum/big_cmp.c\
	  bignum/big_divmod.c\
	  bignum/big_from_chunk.c\
	  bignum/big_inc.c\
	  bignum/big_mul.c\
	  bignum/big_pow.c\
	  bignum/big_shl_shr.c\
	  bignum/big_sub.c\
	  bignum/big_to_str.c\
	  bignum/big_zero.c\
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

