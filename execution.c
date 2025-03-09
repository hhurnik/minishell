//fork the parent process to create a child process, which will be execve()d in order to run a new process, new memory - for example "ls"

// builtin command s execute within the shell process itself
//historically - builtin commands were essential because they enabled the shell to interact efficiently with the environment (like changing directories or controlling job execution) without the overhead of creating separate processes.

/*Since built-in commands interact with the shell’s environment (such as modifying shell variables or handling job control), they need to run within the same process as the shell to easily modify the shell's internal state.

For example, cd changes the current directory of the shell, which is something that only the shell process itself can manage.
exit terminates the shell process.
echo prints directly to the terminal.
These types of operations are best executed in the same process to minimize overhead and allow direct manipulation of the shell’s internal environment.
*/

//external functions like ls/cat - External Programs as Modular Utilities: External commands, like ls, cat, or grep, were intended to be separate programs with their own functionalities. The Unix philosophy emphasized modularity, where the shell itself is just the controller that interacts with various modular utilities. These utilities could be updated, replaced, and used across different programs and users.
//These external commands had their own development and optimization processes, whereas built-ins were created specifically for fast interactions within the shell environment.
/*External programs are independent executables that exist outside the shell. These commands are not aware of the shell’s internal environment by default, so running them involves creating a new process using fork() to execute the command in isolation.

After forking, the shell uses execve() to replace the child process's image with the external command's program.
This process separates the shell's environment from the environment of the external program.
This approach follows the Unix principle of process isolation where each process is self-contained and interacts with others only through defined interfaces (like stdin, stdout, stderr, and files).
*/


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