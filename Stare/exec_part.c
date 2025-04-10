// After parser outputs t_cmd, executor only needs to:
// 1. Loop through the t_cmd list (i.e., handle pipeline structure).
// 2. For each t_cmd, apply its t_redir list before execution.
// 3. Use execve() for external commands.
// 4. Call the builtin handler if it's a builtin.

//TYLKO NOTATKI

///// eval
// cat | cat | cat | ls  - po trzech enterach dopiero zwrot prompta, bo zamykam file descriptory

// nawet gdy nie obsluguje sie options, to argumenty trzeba 

// nawet jesli brak argumentu, to trzeba redirection obsluzyc


// zwrocone struktury
typedef struct s_redir 
{
    int type;       // 0=<, 1=>, 2>> 3<<
    char *file;     // target file or heredoc delimiter
    struct s_redir *next;
} t_redir;

typedef struct s_cmd {
    char **args;
    t_redir *redirs;
    struct s_cmd *next;
} t_cmd;


//helper for redirection
int apply_redirections(t_redir *redirs)
{
    while (redirs)
    {
        if (redirs->type == 0) // <
        {
            int fd = open(redirs->file, O_RDONLY);
            if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
                return perror("minishell"), -1;
            close(fd);
        }
        else if (redirs->type == 1) // >
        {
            int fd = open(redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
                return perror("minishell"), -1;
            close(fd);
        }
        else if (redirs->type == 2) // >>
        {
            int fd = open(redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
                return perror("minishell"), -1;
            close(fd);
        }
        else if (redirs->type == 3) // <<
        {
            if (handle_heredoc(redirs->file) == -1)
                return -1;
        }
        redirs = redirs->next;
    }
    return 0;
}


//pipeline with structure as input
void execute_pipeline(t_cmd *cmd_list)
{
    int prev_fd = -1;
    int pipefd[2];
    pid_t pid;

    while (cmd_list)
    {
        if (cmd_list->next && pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (cmd_list->next)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            // 🔥 Apply redirections here
            if (apply_redirections(cmd_list->redirs) == -1)
                exit(1);

            // Handle builtins inside child only if single command
            if (!cmd_list->next && is_builtin(cmd_list->args[0]))
            {
                execute_builtin(cmd_list->args); // handle exit(), cd etc
                exit(0);
            }

            // External command
            char *cmd_path = find_command_in_path(cmd_list->args[0]);
            if (!cmd_path)
            {
                fprintf(stderr, "minishell: command not found: %s\n", cmd_list->args[0]);
                exit(127);
            }

            execve(cmd_path, cmd_list->args, environ);
            perror("execve");
            exit(1);
        }

        // Parent
        if (prev_fd != -1)
            close(prev_fd);
        if (cmd_list->next)
        {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }

        cmd_list = cmd_list->next;
    }

    while (wait(NULL) > 0);
}
