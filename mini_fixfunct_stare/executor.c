#include "inc/minishell.h"

void cleanup(t_resources *res)
{
    if (res->input)
        free(res->input);
    if (res->tokens)
        free_tokens(res->tokens);
    if (res->cmds)
        free_cmds(res->cmds);
    if (res->env)
        free_env(res->env);
    
    // Clear readline history
    rl_clear_history();
}
//2 leaki
void	process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
		int *pipe_fd)
{
	pid_t pid;

	pid = setup_pipe_and_fork(cmd, pipe_fd);
	if (pid == 0)
		handle_child_process(cmd, env, prev_pipe_read, pipe_fd);
}


int	init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy)
{
	*stdin_copy = dup(STDIN_FILENO);
	*stdout_copy = dup(STDOUT_FILENO);
	if (!cmds || !cmds->args || !cmds->args[0])
	{
		ft_putstr_fd("minishell: null command\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

void	cleanup_execution(int stdin_copy, int stdout_copy)
{
	setup_signals_for_prompt();
	restore_std_fds(stdin_copy, stdout_copy);
}

//moje
void	wait_for_children(void)
{
	int status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}
