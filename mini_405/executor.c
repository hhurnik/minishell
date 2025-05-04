#include "inc/minishell.h"

//moje
void cleanup(t_resources *res)
{
    if (!res) return; // Safety check

    if (res->input)
        free(res->input);
    if (res->tokens)
        free_tokens(res->tokens);
    if (res->cmds)
        free_cmds(res->cmds);
    if (res->env)
        free_env(res->env); // Frees the environment - ONLY call on exit

    rl_clear_history();
    // Nullify pointers in res after freeing to prevent accidental reuse if needed
    // res->input = NULL; res->tokens = NULL; res->cmds = NULL; res->env = NULL;
}

int init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy)
{
    *stdin_copy = dup(STDIN_FILENO);
    *stdout_copy = dup(STDOUT_FILENO);
    if (!cmds || !cmds->args || !cmds->args[0])
    {
        ft_putstr_fd("minishell: null command\n", STDERR_FILENO);
        if (*stdin_copy != -1)
            close(*stdin_copy);
        if (*stdout_copy != -1)
            close(*stdout_copy);
        return (0);
    }
    return (1);
}

void	cleanup_execution(int stdin_copy, int stdout_copy)
{
	setup_signals_for_prompt();
	restore_std_fds(stdin_copy, stdout_copy);
}


void	wait_for_children(pid_t last_cmd_pid)
{
	int status;
	pid_t pid;
	int last_cmd_status = 0; // Store the status of the specific last command
	int found_last = 0;      // Flag to track if we found the last command's status
	int printed_pipe_msg = 0; 

	while ((pid = waitpid(-1, &status, 0)) > 0) // Wait for ANY child
	{
		if (pid == last_cmd_pid)
		{
			found_last = 1;
			if (WIFEXITED(status))
			{
				last_cmd_status = WEXITSTATUS(status);
			}
			else if (WIFSIGNALED(status))
			{
				int signal_num = WTERMSIG(status);
				last_cmd_status = 128 + signal_num;
			}
		}
		if (WIFSIGNALED(status))
		{
			int signal_num = WTERMSIG(status);
			if (signal_num == SIGQUIT)
			{
				ft_putstr_fd("Quit", STDERR_FILENO);
                if (WCOREDUMP(status))
                    ft_putstr_fd(" (core dumped)", STDERR_FILENO);
                ft_putstr_fd("\n", STDERR_FILENO);
			}
			else if (signal_num == SIGSEGV)
			{
				ft_putstr_fd("Segmentation fault", STDERR_FILENO);
                if (WCOREDUMP(status))
                    ft_putstr_fd(" (core dumped)", STDERR_FILENO);
                ft_putstr_fd("\n", STDERR_FILENO);
			}
			else if (signal_num == SIGPIPE)
			{
                if (!printed_pipe_msg)
                {
				    ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
                    printed_pipe_msg = 1;
                }
			}
		}
	}
	if (last_cmd_pid > 0 && found_last)
	{

		g_exit_status = last_cmd_status;
	}
	else if (last_cmd_pid <= 0)
	{
	}
    else // (last_cmd_pid > 0 && !found_last)
    {
    }
	if (pid == -1 && errno != ECHILD)

		perror("minishell: waitpid");
}


