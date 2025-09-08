NAME = minishell

SRC = main.c tokenizer/tokenizer.c general_utils.c parsing.c redirections.c ft_frees.c \
pipe/pipe.c builtings/ft_echo.c builtings/ft_cd.c builtings/ft_execute_builting.c \
builtings/ft_pwd.c ft_environment.c builtings/ft_export.c builtings/ft_env.c \
builtings/ft_unset.c builtings/ft_bexit.c signals/signals.c executions/ft_executions.c \
executions/ft_executions_utils.c ft_getinput.c tokenizer/tokenizer_utils.c
HDR = includes/minishell.h

CC = cc
RM = rm -rf
FLAGS = -Wall -Werror -Wextra -I .  

OBJ = $(SRC:.c=.o)
LIBFT = ./libft/libft.a

all: makelibft $(NAME)

%.o: %.c $(HDR) Makefile $(LIBFT)
			$(CC) $(FLAGS) -c $< -o $@

makelibft: 
	make -C ./libft

$(NAME): $(OBJ) $(libft)
			$(CC) $(FLAGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)
clean: 
	 $(RM) $(OBJ)
	 make clean -C ./libft
fclean:
			$(RM) $(OBJ)
			$(RM) $(NAME)
			make fclean -C ./libft

re: fclean all

.PHONY: all clean fclean re libft

