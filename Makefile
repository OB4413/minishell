NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

FILES = exection_part/minishell.c exection_part/ft_split.c exection_part/additional.c exection_part/additional1.c exection_part/export_and_unset.c exection_part/ft_execvp.c exection_part/ft_strjoin.c exection_part/is_redirection.c \
		parsing-part/minishell.c parsing-part/Tokenization.c parsing-part/Variable_Expansion.c parsing-part/parsing_tokns.c  parsing-part/filde_split.c parsing-part/garbage_collector.c \
		parsing-part/get_next_line.c
OBJECT = $(FILES:.c=.o)
libft = parsing-part/libft/libft.a

all : $(NAME) clean

$(NAME): $(libft) $(OBJECT)
	$(CC) $(CFLAGS) $(OBJECT) $(libft) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(libft):
	make bonus -C parsing-part/libft

clean :
	rm -f $(OBJECT)
	make -C parsing-part/libft clean

fclean : clean
	rm -f $(NAME)
	make -C parsing-part/libft fclean

re : fclean all

.PHONY: all re clean fclean
.SECONDARY: $(OBJECT)
