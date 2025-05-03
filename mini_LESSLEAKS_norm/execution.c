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
		cleanup(&res);
		//free_cmds(cmds);
		//free_env(*env);
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

void handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read,
                          int *pipe_fd)
{
    t_resources res; // Used primarily if bi_exit is called
    int         builtin_status;

    // Initialize resources struct for potential cleanup by bi_exit
    // If the child exits directly here, we use explicit free calls below.
    res.env = *env;
    res.cmds = cmd;    // Child's view of the command(s) it should handle/clean
    res.tokens = NULL; // Tokens usually processed before fork, not relevant here
    res.input = NULL;  // Input string usually processed before fork

    // Set default signal handling for child processes
    signal(SIGINT, SIG_DFL); // Terminate on SIGINT
    signal(SIGQUIT, SIG_DFL); // Terminate and dump core on SIGQUIT

    // --- Setup Input/Output Piping ---
    // Redirect stdin from the previous command's pipe output if applicable
    if (prev_pipe_read != -1)
    {
        if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
        {
            perror("minishell: dup2 stdin");
            free_env(*env); // Cleanup before exit
            free_cmds(cmd); // Cleanup before exit
            exit(EXIT_FAILURE);
        }
        close(prev_pipe_read); // Close the original descriptor
    }

    // Redirect stdout to the current command's pipe input if there's a next command
    if (cmd->next && pipe_fd[1] != -1) // Ensure pipe write end is valid
    {
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
        {
            perror("minishell: dup2 stdout");
            if (pipe_fd[0] != -1) close(pipe_fd[0]); // Close read end if open
            close(pipe_fd[1]); // Close write end
            free_env(*env);    // Cleanup before exit
            free_cmds(cmd);    // Cleanup before exit
            exit(EXIT_FAILURE);
        }
    }

    // Close pipe descriptors that are no longer needed in the child
    if (pipe_fd[0] != -1) // Close the read end of the *current* pipe
        close(pipe_fd[0]);
    if (pipe_fd[1] != -1) // Close the write end of the *current* pipe
        close(pipe_fd[1]);


    // --- Setup File Redirections ---
    if (setup_redirections(cmd->redirections) < 0)
    {
        // setup_redirections should have printed an error message
        // g_exit_status is likely set to 1 by setup_redirections on error
        free_env(*env); // Cleanup child's env copy
        free_cmds(cmd); // Cleanup child's cmd list copy
        exit(1); // Exit with status 1 for redirection errors
    }

    // --- Execute Command ---
    // Check if the command is empty after redirections (e.g., "> file")
    if (!cmd->args || !cmd->args[0])
    {
        free_env(*env); // Nothing to execute, just cleanup
        free_cmds(cmd);
        exit(EXIT_SUCCESS); // No command is not an error in this context
    }

    // Check if it's a builtin command
    if (is_builtin(cmd->args[0]))
    {
        // Run the builtin. run_builtin might call bi_exit which cleans up via 'res' and exits.
        builtin_status = run_builtin(cmd, &res);

        // If run_builtin did NOT exit (i.e., it wasn't bi_exit), we need to clean up.
        // We assume only bi_exit exits internally from run_builtin.
        free_env(*env); // Free child's env copy
        free_cmds(cmd); // Free child's cmd list copy

        exit(builtin_status); // Exit with the status returned by the builtin
    }
    else
    {
        // Execute external command. execute_external MUST handle its own cleanup and exit.
        execute_external(cmd, *env);

        // If execute_external somehow returns (it shouldn't), treat as error.
        ft_putstr_fd("minishell: execute_external returned unexpectedly\n", STDERR_FILENO);
        free_env(*env);
        free_cmds(cmd);
        exit(EXIT_FAILURE); // Should not happen
    }
}

void execute_external(t_cmd *cmd, t_env *env)
{
    char *path = NULL;
    char **env_arr = NULL;
    int exec_errno = 0; // Store errno from execve if it fails

    // Find the executable path
    // Pass the *head* of the env list
    path = get_exec_path(cmd->args[0], env);

    // Convert minishell environment list to char** for execve
    // Pass the *head* of the env list
    env_arr = env_to_arr(env);
    if (!env_arr && errno == ENOMEM) // Check if env_to_arr failed due to malloc
    {
        perror("minishell: failed to allocate environment array");
        // Need to clean up before exiting
        if (path) free(path);
        free_env(env); // Free the t_env list
        free_cmds(cmd); // Free the t_cmd structure
        exit(EXIT_FAILURE); // Indicate general failure
    }


    // --- Case 1: Path not found by get_exec_path ---
    if (!path)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd->args[0], STDERR_FILENO);
        // Check if the original command contained '/' to mimic bash message difference
        if (ft_strchr(cmd->args[0], '/'))
        {
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        }
        else
        {
            ft_putstr_fd(": command not found\n", STDERR_FILENO);
        }

        // Cleanup before exiting
        ft_free_split(env_arr); // Free the environment array
        free_env(env);          // Free the t_env list
        free_cmds(cmd);         // Free the t_cmd structure
        exit(127); // Standard exit code for command not found
    }

    // --- Case 2: Path found, attempt execution ---
    execve(path, cmd->args, env_arr);

    // ---- If execve returns, it failed. ----
    exec_errno = errno; // Save errno immediately

    // --- Report the specific execve error ---
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(cmd->args[0], STDERR_FILENO); // Report error using the original command name
    ft_putstr_fd(": ", STDERR_FILENO);

    int exit_code = 126; // Default exit code for execution errors

    if (exec_errno == ENOENT)
    {
        // This might happen if the path was valid when get_exec_path checked,
        // but was removed before execve, or if path pointed to an interpreter
        // that doesn't exist.
        ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
        exit_code = 127; // Use 127 if execve fails with ENOENT
    }
    else if (exec_errno == EACCES)
    {
        struct stat statbuf;
        // Check if it's a directory *after* EACCES
        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
        {
            ft_putstr_fd("Is a directory\n", STDERR_FILENO);
        }
        else
        {
            ft_putstr_fd("Permission denied\n", STDERR_FILENO);
        }
        exit_code = 126;
    }
    else if (exec_errno == ENOEXEC)
    {
        ft_putstr_fd("Exec format error\n", STDERR_FILENO);
        exit_code = 126;
    }
    // Add other specific errno checks if needed (e.g., ENOMEM)
    else
    {
        // Generic fallback using strerror
        ft_putstr_fd(strerror(exec_errno), STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        exit_code = 126; // Or sometimes 1 for general errors
    }

    // --- Cleanup before exiting due to execve failure ---
    ft_free_split(env_arr); // Free the environment array
    free(path);             // Free the path string
    free_env(env);          // Free the t_env list
    free_cmds(cmd);         // Free the t_cmd structure

    exit(exit_code); // Exit with the determined code
}