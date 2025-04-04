#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/signal.h>
# include <sys/wait.h>
# include <signal.h>
# include <unistd.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

extern int			g_exit_status;

// ------------------------------------------
// Struktury danych
// ------------------------------------------

typedef struct s_env {
    char            *key;
    char            *value;
    struct s_env    *next;
} t_env;

typedef enum e_token_type {
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_APPEND,
    T_HEREDOC
} t_token_type;

typedef enum e_quote {
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} t_quote;

typedef struct s_token {
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

typedef struct s_redir {
    char            *file;
    t_token_type    type;
    struct s_redir  *next;
} t_redir;

typedef struct s_cmd {
    char        **args;
    t_redir     *redirections;
    struct s_cmd *next;
    int         pipe_fd[2];
} t_cmd;

// ------------------------------------------
// Prototypy funkcji
// ------------------------------------------

// Lexer
void            handle_redirections(t_cmd *cmd);
t_token         *tokenize_input(char *input);
void            handle_redirection(char *input, int *i, t_token **tokens);
void            add_token(t_token **tokens, char *value, t_token_type type);
t_token         *handle_quotes(char *input, int *i, t_token *tokens);
t_token         *handle_word(char *input, int *i, t_token *tokens);
int             ft_isspace(char c);
char            *read_quoted_content(char *input, int *i, t_quote quote, int *error);

// Parser
t_cmd           *parse_tokens(t_token *tokens);
void            cmd_add_back(t_cmd **cmds, t_cmd *new_cmd);
t_cmd           *cmd_new(void);
t_cmd           *cmd_last(t_cmd *cmds);
int             is_redirection(t_token_type type);
void            handle_redirection_parser(t_token **tokens, t_cmd *cmd);
void            add_arg_to_cmd(t_cmd *cmd, char *arg);
void            add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir);

// Expander
char            *extract_var_name(char *str);
void            expand_variables(t_token *tokens, t_env *env);
char            *expand_str(char *str, t_env *env);
char            *get_env_value(char *key, t_env *env);

// Executor
void            execute(t_cmd *cmds, t_env **env);
void            close_pipes(t_cmd *cmds);
char            **env_to_arr(t_env *env);
char            *get_exec_path(char *cmd, t_env *env);

// Builtins
int             ft_cd(char **args, t_env **env);
int             is_builtin(char *cmd);
void            run_builtin(t_cmd *cmd, t_env **env);
void            update_pwd_env(t_env **env);
void            run_builtin(t_cmd *cmd, t_env **env);

// Environment
t_env           *init_env(char **envp);
void            env_add_back(t_env **env, t_env *new_node);
t_env           *env_new(char *key, char *value);
char            **env_to_arr(t_env *env);
char            *get_env_value(char *key, t_env *env);

// Utils
char            *ft_strjoin_free(char *s1, char *s2);
char            *ft_strjoin_char(char *s1, char c);
void            free_tokens(t_token *tokens);
void            free_cmds(t_cmd *cmds);
void            ft_free_split(char **split);

// Signals
void            setup_signals(void);
void            handle_sigint(int sig);

// Main
int             main(int argc, char **argv, char **envp);



//builtins Hon
int bi_cd(char **args, t_env **env);
int bi_echo(char *argv[]);
int bi_env(char *argv[]);

void write_stderr(char *str);
int is_integer(char *str);
int bi_exit(char *argv[]);

size_t env_len(t_env *env);
int bi_export(t_env **env);

int bi_pwd(char *argv[]);

int env_cmpr(t_env *env, char *varname);
void remove_env_var(char *varname, t_env **env);
int bi_unset(char **argv, t_env **env);




#endif