#include "inc/minishell.h"

/**
 * Wykonuje komendy, obsługuje potoki i redirekcje.
 */
void execute(t_cmd *cmds, t_env **env) {
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);

    while (cmds) {
        // Potoki
        if (cmds->next) {
            pipe(cmds->pipe_fd);
        }
        // Forkuj dla każdej komendy
        pid_t pid = fork();
        if (pid == 0) {
            handle_redirections(cmds); // Przekieruj wejście/wyjście
            if (is_builtin(cmds->args[0])) {
                run_builtin(cmds, env); // Wbudowane komendy (np. cd)
                exit(g_exit_status);
            } else {
                execve(get_exec_path(cmds->args[0], *env), cmds->args, env_to_arr(*env));
            }
        } else {
            waitpid(pid, &g_exit_status, 0); // Czekaj na dziecko
            close_pipes(cmds); // Zamknij nieużywane pipe'y
        }
        cmds = cmds->next;
    }
    // Przywróć oryginalne stdin/stdout
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
}



void handle_redirections(t_cmd *cmd) {
    t_redir *redir = cmd->redirections;
    while (redir) {
        if (redir->type == T_REDIR_IN) {
            int fd = open(redir->file, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        } else if (redir->type == T_REDIR_OUT) {
            int fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (redir->type == T_APPEND) {
            int fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (redir->type == T_HEREDOC) {
            // Obsługa heredoc (np. tymczasowy plik)
        }
        redir = redir->next;
    }
}

void close_pipes(t_cmd *cmds) {
    while (cmds) {
        if (cmds->pipe_fd[0] != -1) close(cmds->pipe_fd[0]);
        if (cmds->pipe_fd[1] != -1) close(cmds->pipe_fd[1]);
        cmds = cmds->next;
    }
}