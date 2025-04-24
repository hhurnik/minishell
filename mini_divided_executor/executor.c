#include "inc/minishell.h"

void	cleanup(t_env *env, t_token *tokens, t_cmd *cmds, char *input)
{
	// Free readline specific things (might help with *some* readline leaks)
	rl_clear_history(); // Clear history list maintained by readline
	// Consider rl_deprep_terminal() if necessary, but often not needed.
	// Free your data structures
	if (input)
		free(input);
	if (tokens)
		free_tokens(tokens);
	if (cmds)
		free_cmds(cmds);
	if (env)
		free_env(env);
}


/* FUNCTION IMPLEMENTATIONS */

void	process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
	int *pipe_fd)
{
	pid_t	pid;

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

void	wait_for_children(void)
{
	int	status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}
