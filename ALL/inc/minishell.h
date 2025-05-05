/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhurnik <hhurnik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:06:11 by hhurnik           #+#    #+#             */
/*   Updated: 2025/05/05 16:06:11 by hhurnik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/signal.h>
# include <sys/stat.h>
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

extern int	g_exit_status;
// extern volatile sig_atomic_t g_in_command;

// ------------------------------------------
// Struktury danych
// ------------------------------------------

typedef struct s_env
{
	char			*key;
	char			*value;
	int				is_exported; /// added for export, env , pipes ghost=123
	struct s_env	*next;
}	t_env;

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
}	t_token_type;

typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	char			*file;
	t_token_type	type;
	struct s_redir	*next;
	int				heredoc_fd;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirections;
	struct s_cmd	*next;
	int				pipe_fd[2];
}	t_cmd;

typedef struct s_parse_state
{
	int		i;
	int		error;
	int		single_quote_used;
	char	*current_word;
	t_quote	current_quote;
	t_token	*tokens;
}	t_parse_state;

// dla handle_parend ... dodaje poniej
typedef struct s_resources
{
	t_env	*env;
	t_token	*tokens;
	t_cmd	*cmds;
	char	*input;
}	t_resources;

typedef struct s_fds
{
	int	stdin;
	int	stdout;
}	t_fds;

/// dla main_utils2.c
typedef struct s_input_state
{
	char	*input;
	int		*i;
}	t_input_state;

typedef struct s_append_info
{
	int	start;
	int	len;
}	t_append_info;

typedef struct s_quote_indices
{
	int	start;
	int	end_pos;
}	t_quote_indices;

typedef struct s_status_tracker
{
	int	found;
	int	last_status;
}	t_status_tracker;

// ------------------------------------------
// Prototypy funkcji
// ------------------------------------------

// builtin_handler.c
int				is_builtin(char *cmd);
int				run_builtin(t_cmd *cmd, t_resources *res);
int				handle_parent_builtin(t_cmd *cmd, t_fds fds, t_resources *res);
int				handle_single_builtin(t_cmd *cmds, t_resources *res);

// env_utils.c
void			free_env_node(t_env *node);
t_env			*env_new(const char *key, const char *value);
t_env			*get_env(const char *key, t_env *env);
char			*get_env_value(const char *key, t_env *env);
void			env_add_back(t_env **env, t_env *new_node);

// execution_child.c
void			handle_child_process(t_cmd *cmd, t_env **env,
					int prev_pipe_read, int *pipe_fd);
void			setup_child_redirections_or_exit(t_cmd *cmd, t_env **env);
void			redirect_stdin(int fd, t_env **env, t_cmd *cmd);
void			redirect_stdout(int fd, t_env **env, t_cmd *cmd, int *pipe_fd);
void			init_exec_resources(t_resources *res, t_cmd *cmds, t_env **env);

// execution_error.c
void			print_basic_exec_error(int err, t_cmd *cmd);
void			print_exec_access_error(char *path);
void			print_exec_error(int err, t_cmd *cmd, char *path);

// execution_pipes.c
int				setup_pipe_and_fork(t_cmd *cmd, int *pipe_fd);
void			setup_child_pipes(t_cmd *cmd, int prev_pipe_read, int *pipe_fd,
					t_env **env);
void			init_child_resources(t_resources *res, t_cmd *cmd, t_env **env);
void			setup_child_signals(void);
void			exit_child_success(t_env **env, t_cmd *cmd);

// execution.c
void			execute(t_cmd *cmds, t_env **env);
void			execute_external(t_cmd *cmd, t_env *env);
int				prepare_exec_env(t_cmd *cmd, t_env *env, char **path,
					char ***env_arr);
void			handle_execve_failure(t_cmd *cmd, t_env *env, char *path,
					char **env_arr);

// executor_cleanup.c
void			cleanup(t_resources *res);
void			cleanup_execution(int stdin_copy, int stdout_copy);
void			finalize_exit_status(pid_t last_pid, int found,
					int last_status);
void			run_builtin_and_exit(t_cmd *cmd, t_resources *res);

// executor_utils.c
int				count_env_nodes(t_env *env);
char			*create_env_entry(t_env *node);
void			free_array_on_error(char **arr, int count);
int				populate_env_array(t_env *env, char **arr);
char			**env_to_arr(t_env *env);

// executor.c
void			handle_signal_output(int status, int *printed_pipe_msg);
void			process_child_status(pid_t pid, int status, pid_t last_pid,
					t_status_tracker *tracker);
void			process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
					int *pipe_fd);
int				init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy);
void			wait_for_children(pid_t last_cmd_pid);

// expand_str.c
int				append_char_to_result(char **result_ptr, char current_char);
int				process_char_type(char **res, char *str, int i, t_env *env);
bool			run_expansion_loop(char **res, char *str, int *i_ptr,
					t_env *env);
char			*finalize_expansion(char *result, bool error_occurred);
char			*expand_str(char *str, t_env *env);

// expander_var_utils.c
int				is_special_var(char *str, int i);
int				is_braced_var(char *str, int i);
int				is_standard_var(char *str, int i);
char			*extract_var_name(char *str);
void			expand_variables(t_token *tokens, t_env *env);

// expander.c
int				handle_standard_var(char **result, char *str, int i,
					t_env *env);
void			append_char(char **result, char c);
int				handle_special_var(char **result, int i);

// handle_braced_variable.c
int				extract_braced_var_name(char *str, int i,
					char **var_name_ptr, int *end_index_ptr);
int				append_braced_var_value(char **result, char *var_name,
					t_env *env, int end_index);
int				handle_braced_var(char **result, char *str, int i, t_env *env);

// handle_quoted_append_utils.c
int				ensure_word_allocated(char **current_word, int *error);
int				find_closing_quote(t_input_state *input_state, char quote_char);

// handle_quoted_append.c
int				extract_and_append_content(t_append_info *info,
					t_input_state *input_state, char **current_word,
					int *error);
int				prepare_and_execute_append(t_quote_indices *indices,
					t_input_state *input_state, char **current_word,
					int *error);
int				find_and_update_for_quote(t_input_state *input_state,
					int *error, t_parse_state *state,
					t_quote_indices *indices_out);
int				append_and_advance_for_quote(t_input_state *input_state,
					char **current_word, int *error, t_quote_indices *indices);
int				handle_quoted_append(t_input_state *input_state,
					char **current_word, int *error, t_parse_state *state);

// init_env.v
char			*extract_key(char *env_var, int *key_len);
int				handle_env_node_creation(char *key, char *value, t_env **env);
int				process_single_env_var(char *env_var, t_env **env);
void			init_env(char **envp, t_env **env);

// lexer_utils.c
int				is_pipe(char c);
int				is_redirect(char c);
int				is_quote(char c);
t_quote			get_quote_type(char c);
t_token_type	get_token_type_from_quote(t_quote quote);

// lexer_utils2.c
char			*read_quoted_content(char *input, int *i, t_quote quote,
					int *error);
void			free_tokens(t_token *tokens);

// lexer_utils3.c
void			skip_space(int *i, char **word, t_token **tokens);
void			handle_pipe(int *i, char **word, t_token **tokens);
t_token			*handle_unexpected_quote(char *word, t_token *tokens);

// lexer.c
void			add_token(t_token **tokens, char *value, t_token_type type);
t_token			*reverse_tokens(t_token *tokens);

// main_utils.c
void			free_env(t_env *env);
void			init_shell(char **envp, t_env **env);
void			handle_eof(int exit_status);
int				ensure_current_word(char **current_word);
void			append_char_to_word(char **word, char c);

// main_utils2.c
void			finalize_current_word(t_token **tokens, char **current_word,
					int single_quote_used);
void			process_redirection_token(char first_char, char *input, int *i,
					t_token **tokens);
void			handle_redirection(char *input, int *i, t_token **tokens);
t_token			*tokenize_input(char *input);

// main.c
void			handle_no_input(t_env *env);

// parse_input_loop.c
void			handle_redirect_wrapper(char *input, t_parse_state *state);
void			handle_quote_wrapper(char *input, t_parse_state *state);
void			handle_regular_char(char *input, t_parse_state *state);
bool			check_for_comment_break(char *input, t_parse_state *state);
void			parse_input_loop(char *input, t_parse_state *state);

// parse_input_loop_utils.c
void			finalize_and_reset_word(t_parse_state *state);
void			handle_whitespace(t_parse_state *state);
void			handle_pipe_parser(t_parse_state *state);

// parser_utils.c
void			add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir);
t_cmd			*cmd_new(void);
t_cmd			*cmd_last(t_cmd *cmds);
void			cmd_add_back(t_cmd **cmds, t_cmd *new_cmd);
int				is_redirection(t_token_type type);

// parser_utils2.c
void			add_arg_to_cmd(t_cmd *cmd, char *arg);
void			free_cmd_contents(t_cmd *cmd);
void			free_cmds(t_cmd *cmds);

// parser.c
t_redir			*create_redir(t_token *token);
int				check_redir_syntax(t_token *token);
void			handle_redirection_parser(t_token **tokens, t_cmd *cmd);
t_cmd			*process_token(t_token **tokens, t_cmd *current_cmd,
					t_cmd **cmds);
t_cmd			*parse_tokens(t_token *tokens);

// pipess_utils.c
void			cleanup_pipes(int *pipe_fd, int *prev_pipe_read);
void			update_pipe_status(t_cmd *cmd, int *prev_pipe_read,
					int *pipe_fd);

// pipes.c
void			prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read);
void			execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read);
pid_t			process_single_command(t_cmd *cmd, t_env **env,
					int *prev_pipe_read, pid_t last_pid);
void			finalize_pipeline(int *prev_pipe_read, pid_t last_pid);

// process_input_utils.c
void			shift_empty_args(t_cmd *cmds);

// process_input.c
void			process_input(char *input, t_env **env);
bool			initialize_and_tokenize(char *input, t_token **tokens);
bool			prepare_commands(t_token *tokens, t_cmd **cmds,
					t_resources *res);
void			execute_commands(t_cmd *cmds, t_env **env, t_resources *res);

// signals.c
void			handle_sigint_prompt(int sig);
void			handle_sigint_command(int sig);
void			setup_signals_for_prompt(void);
void			setup_signals_for_command(void);

// utils.c
int				ft_isspace(char c);
char			*ft_strjoin_free(char *s1, char *s2);
char			*ft_strjoin_char(char *s, char c);
int				ft_strcmp(const char *s1, const char *s2);

// utils2.c
char			*ft_strjoin3(const char *s1, const char *s2, const char *s3);
void			ft_free_split(char **split);
char			*check_absolute_path(char *cmd);
char			*get_exec_path(char *cmd, t_env *env);
char			*search_in_path_dirs(char *cmd, char **dirs);

//--------------BUILTINS--------------
// bi_cd_special_case.c
int				handle_home_case(t_env **env, char **old_pwd);
int				handle_dash_case(t_env **env, char **old_pwd);
int				process_special_case(char *arg, t_env **env, char **old_pwd);
int				handle_no_args(t_env **env, char **old_pwd);

// bi_cd.c
int				handle_tilde_path(char **path, char *arg, t_env **env,
					char *old_pwd);
int				handle_regular_path(char **path, char *arg, char *old_pwd);
int				process_path_argument(char **args, t_env **env, char **old_pwd);
int				bi_cd(char **args, t_env **env);

// bi_cd_utils.c
void			update_or_create_pwd(t_env **env, t_env *pwd_var,
					char *new_pwd);
void			update_or_create_oldpwd(t_env **env, t_env *oldpwd_var,
					char *old_pwd_copy);
int				update_pwds(t_env **env, char *old_pwd);

// bi_echo.c
int				bi_echo(char *argv[]);
int				handle_n_option(char *argv[], int *newline_flag);

// bi_env.c
int				bi_env(char **args, t_env **env);

// bi_exit.c
void			write_stderr(char *str);
int				is_integer(char *str);
int				parse_exit_args(char *argv[], int *exit_code);
int				bi_exit(char *argv[], t_resources *res);

// bi_export_utils.c
size_t			env_len(t_env *env);
char			*ft_strndup(const char *s, size_t n);
void			print_env_export_format(t_env *env);
int				parse_env_var(const char *var, char **key, char **value);

// bi_export.c
int				update_existing_env(t_env *existing, const char *var,
					char *value);
int				add_new_env(t_env **env, char *key, char *value);
int				set_env_var(const char *var, t_env **env);
int				bi_export(char **args, t_env **env);
int				is_valid_identifier(const char *arg);

// bi_pwd.c
int				bi_pwd(char *argv[]);

// bi_unset.c
int				remove_env_var(char *varname, t_env **env);
int				bi_unset(char **args, t_env **env);

////--------------REDIRECTIONS----------------

// heredoc.c
bool			read_until_delimiter(char *delimiter, int write_fd);
int				handle_heredoc(char *delimiter);
int				process_heredoc_redirection(t_redir *redir);
int				handle_all_heredocs(t_cmd *cmds);
void			close_heredoc_fds(t_cmd *cmds);

// input_output_append.c
int				handle_input_redir(char *file);
int				handle_output_redir(char *file);
int				handle_append_redir(char *file);

// redirs_execute.c
int				validate_redirection(t_redir *redir);
int				handle_redirection_type(t_redir *redir);
int				process_single_redirection(t_redir *redir);
int				setup_redirections(t_redir *redir_list);
void			restore_std_fds(int stdin_copy, int stdout_copy);

// redirs_utils.c
int				create_heredoc_pipe(int pipe_fd[2]);
int				handle_heredoc2(t_redir *redir);
void			print_redirection_error(t_redir *redir);

#endif
