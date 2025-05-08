CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = minishell
FILES = exection_part/minishell.c exection_part/ft_split.c exection_part/additional.c exection_part/additional1.c exection_part/export_and_unset.c exection_part/ft_execvp.c exection_part/ft_strjoin.c exection_part/is_redirection.c \
		parsing-part/minishell.c parsing-part/Tokenization.c parsing-part/Variable_Expansion.c parsing-part/parsing_tokns.c  parsing-part/filde_split.c
OBJECT = $(FILES:.c=.o)
libft = parsing-part/libft/libft.a

all : $(NAME) clean

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME) : $(libft) $(OBJECT)
	$(CC) $(CFLAGS) $(OBJECT) $(libft) -o $(NAME) -lreadline

$(libft):
	make -C parsing-part/libft

clean :
	rm -f $(OBJECT)
	make -C parsing-part/libft clean

fclean : clean
	rm -f $(NAME)
	make -C parsing-part/libft fclean

re : fclean all

.PHONY: all re clean fclean $(NAME)
.SECONDARY: $(OBJECT)
