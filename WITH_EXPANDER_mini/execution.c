#include "inc/minishell.h"

void	init_exec_resources(t_resources *res, t_cmd *cmds, t_env **env)
{
	res->env = *env;
	res->cmds = cmds;
	res->tokens = NULL;
	res->input = NULL;
}

void	execute(t_cmd *cmds, t_env **env)
{
	int			stdin_copy;
	int			stdout_copy;
	int			prev_pipe_read;
	t_resources	res;

	init_exec_resources(&res, cmds, env);
	if (!init_execution(cmds, &stdin_copy, &stdout_copy))
	{
		cleanup(&res);
		return ;
	}
	if (handle_single_builtin(cmds, &res))
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

void	init_child_resources(t_resources *res, t_cmd *cmd, t_env **env)
{
	res->env = *env;
	res->cmds = cmd;
	res->tokens = NULL;
	res->input = NULL;
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_child_pipes(t_cmd *cmd, int prev_pipe_read, int *pipe_fd, t_env **env)
{
	if (prev_pipe_read != -1)
	{
		if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 stdin");
			free_env(*env);
			free_cmds(cmd);
			exit(EXIT_FAILURE);
		}
		close(prev_pipe_read);
	}
	if (cmd->next && pipe_fd[1] != -1)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 stdout");
			if (pipe_fd[0] != -1)
				close(pipe_fd[0]);
			close(pipe_fd[1]);
			free_env(*env);
			free_cmds(cmd);
			exit(EXIT_FAILURE);
		}
	}
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
}
void	handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read, int *pipe_fd)
{
	t_resources	res;
	int			builtin_status;

	init_child_resources(&res, cmd, env);
	setup_child_signals();
	setup_child_pipes(cmd, prev_pipe_read, pipe_fd, env);
	setup_child_redirections_or_exit(cmd, env);
	if (!cmd->args || !cmd->args[0])
	{
		free_env(*env);
		free_cmds(cmd);
		exit(EXIT_SUCCESS);
	}
	if (is_builtin(cmd->args[0]))
	{
		builtin_status = run_builtin(cmd, &res);
		free_env(*env);
		free_cmds(cmd);
		exit(builtin_status);
	}
	execute_external(cmd, *env);
	ft_putstr_fd("minishell: execute_external returned unexpectedly\n", STDERR_FILENO);
	free_env(*env);
	free_cmds(cmd);
	exit(EXIT_FAILURE);
}



void	setup_child_redirections_or_exit(t_cmd *cmd, t_env **env)
{
	if (setup_redirections(cmd->redirections) < 0)
	{
		free_env(*env);
		free_cmds(cmd);
		exit(1);
	}
}


void	execute_external(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	**env_arr;

	path = NULL;
	env_arr = NULL;
	prepare_exec_env(cmd, env, &path, &env_arr);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		if (ft_strchr(cmd->args[0], '/'))
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		else
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ft_free_split(env_arr);
		free_env(env);
		free_cmds(cmd);
		exit(127);
	}
	execve(path, cmd->args, env_arr);
	handle_execve_failure(cmd, env, path, env_arr);
}


int	prepare_exec_env(t_cmd *cmd, t_env *env, char **path, char ***env_arr)
{
	*path = get_exec_path(cmd->args[0], env);
	*env_arr = env_to_arr(env);
	if (!*env_arr && errno == ENOMEM)
	{
		perror("minishell: failed to allocate environment array");
		if (*path)
			free(*path);
		free_env(env);
		free_cmds(cmd);
		exit(EXIT_FAILURE);
	}
	return (0);
}

void	handle_execve_failure(t_cmd *cmd, t_env *env, char *path, char **env_arr)
{
	int	exec_errno;
	int	exit_code;
	struct stat statbuf;

	exec_errno = errno;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->args[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	exit_code = 126;
	if (exec_errno == ENOENT)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		exit_code = 127;
	}
	else if (exec_errno == EACCES)
	{
		if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
			ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		else
			ft_putstr_fd("Permission denied\n", STDERR_FILENO);
	}
	else if (exec_errno == ENOEXEC)
		ft_putstr_fd("Exec format error\n", STDERR_FILENO);
	else
	{
		ft_putstr_fd(strerror(exec_errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	ft_free_split(env_arr);
	free(path);
	free_env(env);
	free_cmds(cmd);
	exit(exit_code);
}
