#include "inc/minishell.h"

void	handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read,
		int *pipe_fd)
{
	t_resources	res;

	init_child_resources(&res, cmd, env);
	setup_child_signals();
	setup_child_pipes(cmd, prev_pipe_read, pipe_fd, env);
	setup_child_redirections_or_exit(cmd, env);
	if (!cmd->args)
		exit_child_success(env, cmd);
	if (!cmd->args[0])
		exit_child_success(env, cmd);
	if (is_builtin(cmd->args[0]))
		run_builtin_and_exit(cmd, &res);
	execute_external(cmd, *env);
	ft_putstr_fd("execute returned unexpectedly\n", STDERR_FILENO);
	free_env(*env);
	free_cmds(cmd);
	exit(EXIT_FAILURE);
}

void	setup_child_redirections_or_exit(t_cmd *cmd, t_env **env)
{
	int	result;

	result = setup_redirections(cmd->redirections);
	if (result < 0)
	{
		free_env(*env);
		free_cmds(cmd);
		exit(1);
	}
}

void	redirect_stdin(int fd, t_env **env, t_cmd *cmd)
{
	int	result;

	result = dup2(fd, STDIN_FILENO);
	if (result == -1)
	{
		perror("minishell: dup2 stdin");
		free_env(*env);
		free_cmds(cmd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	redirect_stdout(int fd, t_env **env, t_cmd *cmd, int *pipe_fd)
{
	int	result;

	result = dup2(fd, STDOUT_FILENO);
	if (result == -1)
	{
		perror("minishell: dup2 stdout");
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		close(fd);
		free_env(*env);
		free_cmds(cmd);
		exit(EXIT_FAILURE);
	}
}

void	init_exec_resources(t_resources *res, t_cmd *cmds, t_env **env)
{
	res->env = *env;
	res->cmds = cmds;
	res->tokens = NULL;
	res->input = NULL;
}
