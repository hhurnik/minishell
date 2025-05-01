#include "inc/minishell.h"

void	execute(t_cmd *cmds, t_env **env)
{
	int	stdin_copy;
	int	stdout_copy;
	int	prev_pipe_read;

	t_resources res = {0}; // Initialize resources structure
	// Populate resources for proper cleanup in case of exit
	res.env = *env;
	res.cmds = cmds;
	// Note: tokens and input would need to be provided from the caller
	// For now, set them to NULL
	res.tokens = NULL;
	res.input = NULL;
	if (!init_execution(cmds, &stdin_copy, &stdout_copy))
	{
		free_cmds(cmds);
		free_env(*env);
		return ;
	}
	// Pass the resources struct to handle_single_builtin
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

void	handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read,
		int *pipe_fd)
{
	t_resources	res;

	res.env = *env;
	res.cmds = cmd;
	res.tokens = NULL; // Tokens are not used here
	res.input = NULL;  // Input is not used here
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
		free_cmds(cmd);
		free_env(*env);
		exit(1);
	}
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, &res)); // Pass res instead of env
	else
		execute_external(cmd, *env);
}

void	execute_external(t_cmd *cmd, t_env *env)
{
	char *path = NULL;
	char **env_arr = NULL;

	path = get_exec_path(cmd->args[0], env);
	env_arr = env_to_arr(env);

	if (!path)
	{
		// get_exec_path couldn't find it (either not in PATH or explicit path didn't exist)
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		// Check if original command contained '/' to mimic bash message difference
		if (ft_strchr(cmd->args[0], '/'))
		{
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		ft_free_split(env_arr);
		// Don't free path, it's NULL
		exit(127);
	}

	// We have a path, attempt execution
	execve(path, cmd->args, env_arr);

	// ---- execve failed if we reach here ----
	int saved_errno = errno;
	struct stat statbuf;

	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		// Report error using the original command name
	ft_putstr_fd(": ", STDERR_FILENO);

	if (saved_errno == ENOENT)
	{
		// This case might be less likely now if get_exec_path uses stat,
		// but keep for robustness / edge cases.
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		ft_free_split(env_arr);
		free(path);
		exit(127);
	}
	else if (saved_errno == EACCES)
	{
		// Check if it's a directory first
		if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		{
			ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd("Permission denied\n", STDERR_FILENO);
		}
		ft_free_split(env_arr);
		free(path);
		exit(126);
	}
	// Add other specific errno checks if needed (e.g.,
	//	ENOEXEC for script format error)
	else
	{
		// Generic fallback
		ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		ft_free_split(env_arr);
		free(path);
		exit(126); // 126 for "command invoked cannot execute" type errors
	}
}