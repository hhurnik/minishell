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

extern int g_exit_status;

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
	T_HEREDOC
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

// Lexer
void	handle_redirections(t_cmd *cmd);
t_token	*tokenize_input(char *input);
void	handle_redirection(char *input, int *i, t_token **tokens);
void	add_token(t_token **tokens, char *value, t_token_type type);
t_token	*handle_quotes(char *input, int *i, t_token *tokens);
t_token	*handle_word(char *input, int *i, t_token *tokens);
int	ft_isspace(char c);
char	*read_quoted_content(char *input, int *i, t_quote quote, int *error);

// Parser
t_cmd	*parse_tokens(t_token *tokens);
void	cmd_add_back(t_cmd **cmds, t_cmd *new_cmd);
t_cmd	*cmd_new(void);
t_cmd	*cmd_last(t_cmd *cmds);
int	is_redirection(t_token_type type);
void	handle_redirection_parser(t_token **tokens, t_cmd *cmd);
void	add_arg_to_cmd(t_cmd *cmd, char *arg);
void	add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir);
void handle_redirection_parser2(t_token **tokens, t_cmd *cmd, t_cmd **cmds_head, t_token *all_tokens);

// Expander
char	*extract_var_name(char *str);
void	expand_variables(t_token *tokens, t_env *env);
char	*expand_str(char *str, t_env *env);
char	*get_env_value(char *key, t_env *env);

// Executor
void	execute(t_cmd *cmds, t_env **env);
void	close_pipes(t_cmd *cmds);
char	**env_to_arr(t_env *env);
char	*get_exec_path(char *cmd, t_env *env);

// Builtins
int	ft_cd(char **args, t_env **env);
int	is_builtin(char *cmd);
//void	run_builtin(t_cmd *cmd, t_env **env);
void	update_pwd_env(t_env **env);
//void	run_builtin(t_cmd *cmd, t_env **env);

// Environment
t_env	*init_env(char **envp);
void	env_add_back(t_env **env, t_env *new_node);
t_env	*env_new(char *key, char *value);
char	**env_to_arr(t_env *env);
char	*get_env_value(char *key, t_env *env);

// Utils
char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_strjoin_char(char *s1, char c);
void	free_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmds);
void	ft_free_split(char **split);

// Signals
void	setup_signals(void);
void	handle_sigint(int sig);

// Main
int	main(int argc, char **argv, char **envp);

// builtins Hon
int bi_cd(char *comment, t_env **env); //inaczej nazwac
int	bi_echo(char *argv[]);
char *get_env_value2(const char *key, t_env *env);
int	bi_env(char *argv[]);

void	write_stderr(char *str);
int	is_integer(char *str);
int	bi_exit(char *argv[], t_env *env);

size_t	env_len(t_env *env);
int	bi_export(t_env **env);

int	bi_pwd(char *argv[]);

int	env_cmpr(t_env *env, char *varname);
void	remove_env_var(char *varname, t_env **env);
int	bi_unset(char **argv, t_env **env);
void	update_pwd_env(t_env **env);
int run_builtin(t_cmd *cmd, t_env **env);


void	free_tokens(t_token *tokens);

//size_t get_length_with_space(char *str, const char *cmd);
int handle_special_paths(char *arg, t_env **env, char *old_pwd);
int change_directory(char *path, t_env **env, char *old_pwd);
int print_error(const char *file, int line, int return_code, ...);
int update_pwds(t_env **env, char *old_pwd);
t_env *get_env(const char *key, t_env *env);
//size_t get_length_with_space(const char *input, const char *cmd);
int handle_special_paths(char *arg, t_env **env, char *old_pwd);
int handle_variable_path(char *var_name, t_env **env, char *old_pwd);
int handle_oldpwd_case(t_env **env, char *old_pwd);
int change_to_path(const char *path, t_env **env, char *old_pwd);
int print_error(const char *file, int line, int return_code, ...);
size_t get_length_with_space(char *str, const char *cmd);



//heredoc file - with redirections
int	handle_input_redir(char *file);
int	handle_output_redir(char *file);
int	handle_append_redir(char *file);
int handle_heredoc(const char *delimiter);
int	handle_heredoc_redir(char *delimiter);
int	setup_redirections(t_redir *redir_list);
void free_env_list(t_env *env);

//env utils
void init_env2(char **envp, t_env **env);


//executor

void free_env_arr(char **arr);
char **env_to_arr(t_env *env);
char *ft_strjoin_three(const char *s1, const char *s2, const char *s3);
int env_size(t_env *env);


void cleanup(t_env *env, t_token *tokens, t_cmd *cmds, char *input);


#endif

///valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main