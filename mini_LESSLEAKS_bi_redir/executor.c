#include "inc/minishell.h"


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
	int printed_pipe_msg = 0; // Flag to potentially avoid printing "Broken pipe" multiple times

	// Initial status if last_cmd_pid is invalid (e.g., fork failed before loop)
	// If fork failed for the last command, execution might have already set
	// g_exit_status to an error code (like 1). We should preserve that if
	// no children are successfully waited for.
	// If last_cmd_pid <= 0, we can't rely on found_last/last_cmd_status later.

	while ((pid = waitpid(-1, &status, 0)) > 0) // Wait for ANY child
	{
		// --- Check status of the specific last command ---
		if (pid == last_cmd_pid)
		{
			found_last = 1; // Mark that we found it
			if (WIFEXITED(status))
			{
				last_cmd_status = WEXITSTATUS(status); // Get its normal exit status
			}
			else if (WIFSIGNALED(status)) // If the last command itself was signaled
			{
				int signal_num = WTERMSIG(status);
				last_cmd_status = 128 + signal_num; // Calculate its signal exit status
                // Don't print signal messages here yet, handle below for consistency
			}
            // else: Handle other termination types if necessary (stopped, continued)
		}

		// --- Check for signaled termination for *any* child waited for in this loop ---
		// This is where we print messages like "Broken pipe", "Quit", etc.
		if (WIFSIGNALED(status))
		{
			int signal_num = WTERMSIG(status);

            // Print signal messages like bash - DO THIS FOR *ANY* CHILD THAT SIGNALED
            // We print these messages even if the process wasn't the 'last_cmd_pid'
			if (signal_num == SIGQUIT)
			{
				// Bash prints "Quit (core dumped)" - adjust as needed
				// Check WCOREDUMP(status) if you want to add "(core dumped)"
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
			else if (signal_num == SIGPIPE) // SIGPIPE is usually signal 13
			{
                // Optional: Only print "Broken pipe" once per pipeline wait
                if (!printed_pipe_msg)
                {
				    ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
                    printed_pipe_msg = 1; // Set flag after printing
                }
                // Note: If multiple processes get SIGPIPE concurrently, Bash might
                // technically print it multiple times, but often only one matters.
                // Printing once is usually sufficient for mimicking the common case.
			}
            // else if (signal_num == SIGINT) {
            //     // We usually don't print anything extra for SIGINT here,
            //     // as the newline is typically handled by the signal handler.
            //     // The exit status (130) will be set correctly if it was the last command.
            // }
			// Add checks for other signals if needed
		}
		// We continue waiting even after finding the last command to ensure
		// all children in the pipeline are reaped and no zombies are left.
	} // End of while loop

	// --- Set the final global exit status ---
	// After waiting for all children, set the global exit status based
	// on the termination status of the *last* command in the pipeline.
	if (last_cmd_pid > 0 && found_last)
	{
		// We successfully found and waited for the last command.
		// last_cmd_status contains either its exit code or signal status (128+sig).
		g_exit_status = last_cmd_status;
	}
	else if (last_cmd_pid <= 0)
	{
		// Handle cases where there was no valid last command pid
		// (e.g., single command, fork failure before loop).
		// In this scenario, g_exit_status should likely retain the value
		// it had before wait_for_children was called (e.g., error status
		// from setup, or status from a single builtin).
		// If waitpid returned ECHILD immediately (no children), we also fall here.
		// So, we explicitly *don't* modify g_exit_status here.
	}
    else // (last_cmd_pid > 0 && !found_last)
    {
        // This case is strange: We had a valid last PID, but waitpid finished
        // without finding it (perhaps waitpid failed with EINTR or another error).
        // It might be safer to not modify g_exit_status or set it to a generic error.
        // Let's leave g_exit_status as is for now.
    }


	// --- Error handling for waitpid itself ---
	if (pid == -1 && errno != ECHILD)
	{
		// waitpid failed for a reason other than "no child processes"
		perror("minishell: waitpid");

	}
}


