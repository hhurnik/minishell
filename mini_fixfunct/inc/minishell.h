#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <sys/signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// constants
# define SUCCESS 0
# define ERROR 1
# define CMD_NOT_FOUND 127     // moze do skorzystania
# define PERMISSION_DENIED 126 // same
# define ERROR_CD -1           // usunac?
# define DOLLAR '$'
# define CONTINUE 2

extern int g_exit_status;
// extern volatile sig_atomic_t g_in_command;

// ------------------------------------------
// Struktury danych
// ------------------------------------------

typedef struct s_env
{
	char *key;
	char *value;
	int is_exported; /// added for export, env , pipes ghost=123
	struct s_env *next;
} t_env;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC,
	T_SINGLE_QUOTED,
	T_DOUBLE_QUOTED,
} t_token_type;

typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
} t_quote;

typedef struct s_token
{
	char *value;
	t_token_type type;
	struct s_token *next;
} t_token;

typedef struct s_redir
{
	char *file;
	t_token_type type;
	struct s_redir *next;
} t_redir;

typedef struct s_cmd
{
	char **args;
	t_redir *redirections;
	struct s_cmd *next;
	int pipe_fd[2];
} t_cmd;

typedef struct s_parse_state
{
	int i;
	int error;
	int single_quote_used;
	char *current_word;
	t_quote current_quote;
	t_token *tokens;
} t_parse_state;

// dla handle_parend ... dodaje poniej
typedef struct s_resources
{
	t_env *env;
	t_token *tokens;
	t_cmd *cmds;
	char *input;
} t_resources;

typedef struct s_fds
{
	int stdin;
	int stdout;
} t_fds;

// ------------------------------------------
// Prototypy funkcji
// ------------------------------------------

////podzielony executor.c:  /////////////////////
// redirs_execute.c
int	process_single_redirection(t_redir *redir);
int	setup_redirections(t_redir *redir_list);
void	restore_std_fds(int stdin_copy, int stdout_copy);

// execution.c
int	setup_pipe_and_fork(t_cmd *cmd, int *pipe_fd);
void	handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read,
		int *pipe_fd);

void	execute_external(t_cmd *cmd, t_env *env);
// void	execute(t_cmd *cmds, t_env **env);
void	execute(t_cmd *cmds, t_env **env);

// executor.c
// void	cleanup(t_env *env, t_token *tokens, t_cmd *cmds, char *input);
void	cleanup(t_resources *res);
void	process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
		int *pipe_fd);
int	init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy);
void	cleanup_execution(int stdin_copy, int stdout_copy);
// void	wait_for_children(void);
void	wait_for_children(pid_t last_cmd_pid);

// pipes.c
void	cleanup_pipes(int *pipe_fd, int *prev_pipe_read);
void	prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read);
void	execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read);
// void execute_pipeline(t_cmd *cmds, t_resources *res, int *prev_pipe_read);
void	update_pipe_status(t_cmd *cmd, int *prev_pipe_read, int *pipe_fd);

// i builtinhandler

/////////////koniec podzielonego executora

// builtin_handler.c
int	is_builtin(char *cmd);
// int		run_builtin(t_cmd *cmd, t_env **env);
int	run_builtin(t_cmd *cmd, t_resources *res);
// int		handle_parent_builtin(t_cmd *cmd, t_env **env, int stdin_copy,
// 			int stdout_copy);
int	handle_parent_builtin(t_cmd *cmd, t_fds fds, t_resources *res);

// int		handle_single_builtin(t_cmd *cmds, t_env **env, int stdin_copy,
// 			int stdout_copy);
int	handle_single_builtin(t_cmd *cmds, t_resources *res);

// env_utils.c
t_env	*get_env(const char *key, t_env *env);
char	*get_env_value(const char *key, t_env *env);
t_env	*env_new(const char *key, const char *value);
void	env_add_back(t_env **env, t_env *new_node);

// init_env.c
char	*extract_key(char *env_var, int *key_len);
int	handle_env_node_creation(char *key, char *value, t_env **env);
int	process_single_env_var(char *env_var, t_env **env);
void	init_env(char **envp, t_env **env);

// executor_utils.c
int	count_env_nodes(t_env *env);
char	*create_env_entry(t_env *node);
void	free_array_on_error(char **arr, int count);
int	populate_env_array(t_env *env, char **arr);
char	**env_to_arr(t_env *env);

// expander.c
char	*extract_var_name(char *str);
void	expand_variables(t_token *tokens, t_env *env);
int	handle_braced_var(char **result, char *str, int i, t_env *env);
int	handle_standard_var(char **result, char *str, int i, t_env *env);
char	*expand_str(char *str, t_env *env);
// char	*remove_quotes(char *str, char quote_type);

// expander_utils.c
int	is_special_var(char *str, int i);
int	is_braced_var(char *str, int i);
int	is_standard_var(char *str, int i);
void	append_char(char **result, char c);
int	handle_special_var(char **result, int i);

// lexer.c
void	add_token(t_token **tokens, char *value, t_token_type type);
t_token	*reverse_tokens(t_token *tokens);
void	parse_input_loop(char *input, t_parse_state *state);
t_token	*tokenize_input(char *input);
void	handle_input_char(char *input, int *i, char **current_word,
		t_token **tokens);

// lexer_utils.c
int	is_pipe(char c);
int	is_redirect(char c);
int	is_quote(char c);
t_quote	get_quote_type(char c);
t_token_type	get_token_type_from_quote(t_quote quote);

// lexer_utils2.c
void finalize_current_word(t_token **tokens, char **current_word, int single_quote_used);

int	handle_quote(char *input, int *i, t_token **tokens, int *error);
char	*read_quoted_content(char *input, int *i, t_quote quote, int *error);
void	free_tokens(t_token *tokens);

// lexer_utils3.c
void	skip_space(int *i, char **word, t_token **tokens);
void	handle_pipe(int *i, char **word, t_token **tokens);

// int	handle_quoted(char *input, int *i, char **current_word, int *error);
int	handle_quoted(char *input, int *i, t_token **tokens, int *error);

void	append_char_to_word(char **word, char c);
t_token	*handle_unexpected_quote(char *word, t_token *tokens);

// lexer_utils4.c
void	handle_redirect_token(char *input, int *i, char **word,
		t_token **tokens);
void	validate_redirection_syntax(t_token **tokens);
void	handle_redirection(char *input, int *i, t_token **tokens);

// main_utils.c
void	free_env(t_env *env);
void	init_shell(char **envp, t_env **env);
// void	handle_eof(t_env *env, int exit_status);
void	handle_eof(int exit_status);
// void	process_input(char *input, t_env **env);
void	process_input(char *input, t_env **env);
void	clean_cmds_tokens_input(t_cmd *cmds, t_token *tokens, char *input);

// parser_utils.c
void	add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir);
t_cmd	*cmd_new(void);
t_cmd	*cmd_last(t_cmd *cmds);
void	cmd_add_back(t_cmd **cmds, t_cmd *new_cmd);
int	is_redirection(t_token_type type);

// parser_utils2.c
void	add_arg_to_cmd(t_cmd *cmd, char *arg);

// parser.c
void	handle_redirection_parser(t_token **tokens, t_cmd *cmd);
void	free_cmd_contents(t_cmd *cmd);
void	free_cmds(t_cmd *cmds);
t_cmd	*process_token(t_token **tokens, t_cmd *current_cmd, t_cmd **cmds);
t_cmd	*parse_tokens(t_token *tokens);

// signals.c
void	handle_sigint_prompt(int sig);
void	handle_sigint_command(int sig);
void	setup_signals_for_prompt(void);
void	setup_signals_for_command(void);

// utils.c
int	ft_isspace(char c);
char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_strjoin_char(char *s, char c);
int	ft_strcmp(const char *s1, const char *s2);

// utils2.c
char	*ft_strjoin3(const char *s1, const char *s2, const char *s3);
char	*check_absolute_path(char *cmd);
char	*get_exec_path(char *cmd, t_env *env);
char	*search_in_path_dirs(char *cmd, char **dirs);
void	ft_free_split(char **split);

//--------------BUILTINS--------------
// bi_cd_special_case.c
int	handle_home_case(t_env **env, char **old_pwd);
int	handle_dash_case(t_env **env, char **old_pwd);
int	process_special_case(char *arg, t_env **env, char **old_pwd);
int	handle_no_args(t_env **env, char **old_pwd);

// bi_cd.c
int	handle_tilde_path(char **path, char *arg, t_env **env, char *old_pwd);
int	handle_regular_path(char **path, char *arg, char *old_pwd);
int	process_path_argument(char **args, t_env **env, char **old_pwd);
int	bi_cd(char **args, t_env **env);

// bi_cd_utils.c
void	update_or_create_pwd(t_env **env, t_env *pwd_var, char *new_pwd);
void	update_or_create_oldpwd(t_env **env, t_env *oldpwd_var,
		char *old_pwd_copy);
int	update_pwds(t_env **env, char *old_pwd);

// bi_echo.c
int	bi_echo(char *argv[]);
int	handle_n_option(char *argv[], int *newline_flag);

// bi_env.c
int	bi_env(char **args, t_env **env);

// bi_exit.c
void	write_stderr(char *str);
int	is_integer(char *str);
int	parse_exit_args(char *argv[], int *exit_code);
// int		bi_exit(char *argv[], t_env *env);
int	bi_exit(char *argv[], t_resources *res);
// bi_export.c
size_t	env_len(t_env *env);
void	print_env_export_format(t_env *env);
int	parse_env_var(const char *var, char **key, char **value);
int	set_env_var(const char *var, t_env **env);
int	bi_export(char **args, t_env **env);

// bi_pwd.c
int	bi_pwd(char *argv[]);

// bi_unset.c
// int		is_valid_identifier(char *name);
int	is_valid_identifier(const char *arg);
int	remove_env_var(char *varname, t_env **env);
int	bi_unset(char **args, t_env **env);

////--------------REDIRECTIONS----------------
// input_output_append.c
int	handle_input_redir(char *file);
int	handle_output_redir(char *file);
int	handle_append_redir(char *file);

// heredoc.c
int	create_heredoc_pipe(int pipe_fd[2]);
void	process_heredoc_line(int write_fd, char *line);
int	handle_heredoc(const char *delimiter);
int	handle_heredoc_redir(char *delimiter);

// int handle_quoted_token(char *input, int *i, t_token **tokens, int *error);
// int append_char_to_word(char **word, char c);
#endif
