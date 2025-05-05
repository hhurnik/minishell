NAME = minishell
CC = gcc
CFLAGS = -g -Wall -Werror -Wextra
CPPFLAGS = -Iinc -Ilibft/includes
LDLIBS = -lreadline
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

BUILTINS_SRCS = builtins/bi_cd_special_case.c \
				builtins/bi_cd_utils.c \
				builtins/bi_cd.c \
				builtins/bi_echo.c \
				builtins/bi_env.c \
				builtins/bi_exit.c \
				builtins/bi_export_utils.c \
				builtins/bi_export.c \
				builtins/bi_pwd.c \
				builtins/bi_unset.c

REDIR_SRCS = redirections/heredoc.c \
				redirections/input_output_append.c \
				redirections/redirs_execute.c \
				redirections/redirs_utils.c

SRC_FILES = builtin_handler.c \
			env_utils.c \
			execution_child.c \
			execution_error.c \
			execution_pipes.c \
			execution.c \
			executor_cleanup.c \
			executor_utils.c \
			executor.c \
			expand_str.c \
			expander_var_utils.c \
			expander.c \
			handle_braced_variable.c \
			handle_quoted_append_utils.c \
			handle_quoted_append.c \
			handle_standard_var.c \
			init_env.c \
			lexer_utils.c \
			lexer_helpers.c \
			lexer_utils_token.c \
			lexer.c \
			main_utils.c \
			main_utils_lexer.c \
			main.c \
			parse_input_loop_utils.c \
			parse_input_loop.c \
			parser_utils.c \
			parser_utils_cmd.c \
			parser.c \
			pipes_utils.c \
			pipes.c \
			process_input_utils.c \
			process_input.c \
			signals.c \
			utils.c \
			utils_dir.c

SRCS := $(BUILTINS_SRCS) $(REDIR_SRCS) $(SRC_FILES)
OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)  $(LIBFT) $(LDLIBS) -o $(NAME)

%.o: %.c inc/minishell.h
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
