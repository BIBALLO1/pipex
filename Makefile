SRC = pipex.c
OBJ = $(SRC:.c=.o)

NAME = pipex

LIBDIR = libft
LIB = $(LIBDIR)/libft.a
LIBFLAGS = -L$(LIBDIR) -lft

CFLAGS = -Wall -Wextra -Werror # -fsanitize=address -g3

all: $(NAME)

$(NAME): $(LIB) $(OBJ)
	cc $(CFLAGS) $(OBJ) $(LIBFLAGS) -o $@

$(LIB):
	@$(MAKE) --no-print-directory -C $(LIBDIR)

%.o: %.c
	cc $(CFLAGS) -I$(LIBDIR) -c $< -o $@

clean:
	@$(MAKE) --no-print-directory -C libft clean
	@rm -f $(OBJ)

fclean: clean
	@$(MAKE) --no-print-directory -C libft fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
