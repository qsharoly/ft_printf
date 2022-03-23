NAME = libftprintf.a
INCDIR = includes 
OBJDIR = obj
SRCDIR = src
LIBFT = libft
LFT_INCDIR = $(LIBFT)/includes
CC = gcc
CCFLAGS += -Wall -Wextra -Werror -O2
INCFLAGS = -I$(INCDIR) -I$(LFT_INCDIR)

#Disable union ABI warning on linux gcc
UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S), Linux)
ifeq ($(CC), gcc)
	CCFLAGS += -Wno-psabi
endif
endif

debug = yes
ifeq ($(debug), yes)
	CCFLAGS += -g
endif

SRC = ft_printf.c\
	  ft_snprintf.c\
	  sv.c\
	  pf_stream.c\
	  pf_stream_helpers.c\
	  pf_parse.c\
	  pf_conv_basic.c\
	  pf_conv_putnbr.c\
	  pf_conv_diuox.c\
	  pf_conv_floating.c\
	  pf_utoa_base.c\
	  pf_dtoa.c\
	  pf_dtoa_quick.c\
	  bignum/big_zero.c\
	  bignum/big_from_digit.c\
	  bignum/big_add.c\
	  bignum/big_mul.c\
	  bignum/big_raise.c\
	  bignum/big_to_str.c\
	  float/ft_fabs.c\
	  float/ft_isneg.c\
	  float/ft_isinf.c\
	  float/ft_isnan.c\
	  float/ft_issub.c\
	  float/ft_trunc.c

OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

$(shell mkdir -p $(OBJDIR) $(OBJDIR)/bignum $(OBJDIR)/float)


all: $(NAME)

export debug
export CC
$(NAME): $(OBJ)
	make -C $(LIBFT)
	@echo "# packing $(NAME)"
	cp $(LIBFT)/libft.a ./$(NAME)
	ar rcs $(NAME) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c includes/libftprintf.h includes/sv.h
	$(CC) -c $< -o $@ $(INCFLAGS) $(CCFLAGS)
$(OBJDIR)/pf_conv_floating.o: includes/float.h
$(OBJDIR)/pf_dtoa_quick.o: includes/float.h
$(OBJDIR)/pf_dtoa.o: includes/bignum.h includes/float.h
$(OBJDIR)/bignum/*.o: includes/bignum.h
$(OBJDIR)/float/*.o: includes/float.h

.PHONY: all clean fclean re

clean:
	rm -rf $(OBJDIR)
	make -C $(LIBFT) clean
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT) fclean
re: fclean
	make all
