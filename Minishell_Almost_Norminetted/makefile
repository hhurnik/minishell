NAME 				= minishell
CC					= gcc -g -Wall -Werror -Wextra -I
LIBFT 				= libft/libft.a
RL_FLAGS 			= -lreadline 
INC					= inc/
SRCS 				= *.c builtins/*.c redirections/*.c

start:
					@make all
$(LIBFT):
					@make -C libft/


all: 				$(NAME)

$(NAME): 			$(LIBFT) 
					$(CC) $(INC) $(SRCS) $(LIBFT) -lreadline -o $(NAME)

clean:
					@$(RM) -rf *.o
					@make clean -C libft/

fclean: 			clean
					@$(RM) $(NAME)
					@$(RM) $(LIBFT)

re: 				fclean all

.PHONY: 			start all clean fclean re
