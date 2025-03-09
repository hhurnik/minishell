int execute_builtin(t_cmd *cmd, t_shell *shell)
{
    if (!cmd->args || !cmd->args[0])
        return 0;

    if (strcmp(cmd->args[0], "echo") == 0)
        return builtin_echo(cmd->args);
    
    // inne polecenia wbudowane...
    
    return (1);
}





void execute_command(t_shell *shell)
{
    if (!shell || !shell->cmd)
        return;

    if (!shell->cmd->next && is_builtin(shell->cmd->args[0]))
    {
        // TODO: wykonaj polecenie wbudowane
    }
    else
    {
        // TODO: wykonaj potok
    }
}

void execute_pipeline(t_shell *shell)
{
    t_cmd *cmd = shell->cmd;
    int prev_pipe = -1;

    while (cmd)
    {
        // TODO: utwórz pipe, fork, wykonaj polecenie
        cmd = cmd->next;
    }

    // TODO: czekaj na ostatniego potomka
}

int setup_redirections(t_cmd *cmd, int saved_fds[2])
{
    // TODO: zapisz stdin i stdout
    // TODO: otwórz pliki i wykonaj dup2()
    return 0;
}

void restore_fds(int saved_fds[2])
{
    // TODO: przywróć stdin i stdout
}


void execute_external_command(t_cmd *cmd, t_shell *shell)
{
    // TODO: znajdź ścieżkę do polecenia
    // TODO: użyj execve
}


int is_builtin(char *cmd)
{
    // TODO: sprawdzaj nazwy wbudowanych poleceń
    return 0;
}

void setup_child_process(t_cmd *cmd, int prev_pipe, int pipe_fd[2])
{
    // TODO: redirekcje, dup2 dla pipe
}


void handle_sigint(int signum)
{
    // TODO: obsłuż Ctrl+C
}

void setup_signals(void)
{
    // TODO: ustal obsługę SIGINT
}