#include "inc/minishell.h"

void	execute(t_cmd *cmds, t_env **env)
{
	int	stdin_copy;
	int	stdout_copy;
	int	prev_pipe_read;

	if (!init_execution(cmds, &stdin_copy, &stdout_copy))
	{
		free_cmds(cmds); //dodane valgrind
		free_env(*env); //dodane valgrind
		return ;
	}
	if (handle_single_builtin(cmds, env, stdin_copy, stdout_copy))
		return ;
	prepare_pipeline_execution(cmds, &prev_pipe_read);
	execute_pipeline(cmds, env, &prev_pipe_read);
	cleanup_execution(stdin_copy, stdout_copy);
}

int	setup_pipe_and_fork(t_cmd *cmd, int *pipe_fd)
{
	pid_t	pid;

	if (cmd->next && pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		return (0);
	}
	return (pid);
}

void	handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read,
	int *pipe_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_pipe_read != -1)
	{
		dup2(prev_pipe_read, STDIN_FILENO);
		close(prev_pipe_read);
	}
	if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	if (setup_redirections(cmd->redirections) < 0)
	{
		free_cmds(cmd); //dodane valgrind
		free_env(*env); //dodane valgrind
		exit(1);
	}
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, env));
	else
		execute_external(cmd, *env);
}

void	execute_external(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	**env_arr;

	path = get_exec_path(cmd->args[0], env);
	env_arr = env_to_arr(env);
	if (path)
		execve(path, cmd->args, env_arr);
	else
		execvp(cmd->args[0], cmd->args);
	perror("minishell");
	exit(127);
}
