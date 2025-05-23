NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

FILES = exection_part/minishell.c exection_part/ft_split.c exection_part/additional.c exection_part/additional1.c \
		exection_part/ft_strjoin.c exection_part/is_redirection.c exection_part/additional2.c exection_part/builtings.c \
		exection_part/export_and_unset.c exection_part/ft_execvp.c exection_part/additional3.c exection_part/additional4.c \
		parsing-part/minishell.c parsing-part/Tokenization.c parsing-part/Variable_Expansion.c parsing-part/parsing_tokns.c \
		parsing-part/ga_coll_and_utils.c parsing-part/utils1.c parsing-part/utils2.c parsing-part/utils3.c parsing-part/utils4.c\
		parsing-part/utils5.c parsing-part/utils6.c parsing-part/utils7.c
OBJECT = $(FILES:.c=.o)
libft = parsing-part/libft/libft.a

all : $(NAME)

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
