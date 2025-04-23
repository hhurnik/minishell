#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
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
#define ERROR_cd -1
#define SUCCESS 0
#define DOLLAR '$'
#define CONTINUE 2

extern int g_exit_status;
//extern volatile sig_atomic_t g_in_command;


// ------------------------------------------
// Struktury danych
// ------------------------------------------

typedef struct s_env
{
	char *key;
	char *value;
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

// ------------------------------------------
// Prototypy funkcji
// ------------------------------------------

//builtin_handler.c
int	is_builtin(char *cmd);
int	execute_builtin(char **args, t_env **env);
int run_builtin(t_cmd *cmd, t_env **env);


//env_utils.c
t_env *env_new_no_dup(char *key, char *value) ;
void	env_add_back(t_env **env, t_env *new_node);
t_env	*init_env(char **envp);
void init_env2(char **envp, t_env **env);
void free_env_list(t_env *env);
char	*get_env_value(char *key, t_env *env);
void	update_pwd_env(t_env **env);


//executor_utils.c
char	**env_to_arr(t_env *env);


//executor.c
void	restore_std_fds(int stdin_copy, int stdout_copy);
void execute(t_cmd *cmds, t_env **env);
void	handle_redirections(t_cmd *cmd);
void	close_pipes(t_cmd *cmds);
void cleanup(t_env *env, t_token *tokens, t_cmd *cmds, char *input);


//expander.c
char	*extract_var_name(char *str);
void expand_variables(t_token *tokens, t_env *env);
char	*expand_str(char *str, t_env *env);


//lexer.c
void	add_token(t_token **tokens, char *value, t_token_type type);
t_token	*reverse_tokens(t_token *tokens);
void	free_tokens(t_token *tokens);
char	*read_quoted_content(char *input, int *i, t_quote quote, int *error);
t_token	*tokenize_input(char *input);
void handle_redirection(char *input, int *i, t_token **tokens);

//main.c
void free_env(t_env *env);


//parser_utils.c
void	add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir);
t_cmd	*cmd_new(void);
t_cmd	*cmd_last(t_cmd *cmds);
void	cmd_add_back(t_cmd **cmds, t_cmd *new_cmd);
int	is_redirection(t_token_type type);
void add_arg_to_cmd(t_cmd *cmd, char *arg);

//parser.c
void	handle_redirection_parser(t_token **tokens, t_cmd *cmd);
void	print_parsed_commands(t_cmd *cmds);  ////for debugging, to be removed
void	free_cmds(t_cmd *cmds);
t_cmd	*parse_tokens(t_token *tokens);

//signals.c
void handle_sigint(int sig);
void	setup_signals(void);

void setup_signals_for_command(void);
void setup_signals_for_prompt(void); 

//utils.c
int	ft_isspace(char c);
char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_strjoin_char(char *s, char c);
int	ft_strcmp(const char *s1, const char *s2);


//utils2.c
char	*ft_strjoin3(const char *s1, const char *s2, const char *s3);
char	*get_exec_path(char *cmd, t_env *env);
void	ft_free_split(char **split);


//--------------BUILTINS--------------

//bi_cd.c
t_env *env_new(const char *key, const char *value);
t_env	*get_env(const char *key, t_env *env);
char	*get_env_value5(const char *key, t_env *env);
int	update_pwds(t_env **env, char *old_pwd);
int	handle_home_case(t_env **env, char **old_pwd);
int	handle_dash_case(t_env **env, char **old_pwd);
int	process_special_case(char *arg, t_env **env, char **old_pwd);
int			bi_cd(char **args, t_env **env);


//bi_echo.c
int	bi_echo(char *argv[]);

//bi_env.c
int bi_env(char **args, t_env **env);

//bi_exit.c
void	write_stderr(char *str);
int	is_integer(char *str);
int bi_exit(char *argv[], t_env *env);

//bi_export.c
size_t env_len(t_env *env);
size_t env_key_len(t_env *env);
void print_env_export_format(t_env *env);
int set_env_var(const char *var, t_env **env);
int bi_export(char **args, t_env **env);

//bi_pwd.c
int	bi_pwd(char *argv[]);

//bi_unset.c
int is_valid_identifier(char *name);
int remove_env_var(char *varname, t_env **env);
int bi_unset(char **args, t_env **env);



////--------------REDIRECTIONS----------------
int	handle_input_redir(char *file);
int	handle_output_redir(char *file);
int	handle_append_redir(char *file);
int handle_heredoc(const char *delimiter);
int	handle_heredoc_redir(char *delimiter);
int	setup_redirections(t_redir *redir_list);


















#endif

///valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main