#include "../inc/minishell.h"

void	restore_std_fds(int stdin_copy, int stdout_copy)
{
	// Ensure descriptors are valid before dup2
	if (stdin_copy >= 0)
	{
		dup2(stdin_copy, STDIN_FILENO);
		close(stdin_copy);
	}
	if (stdout_copy >= 0)
	{
		dup2(stdout_copy, STDOUT_FILENO);
		close(stdout_copy);
	}
}

int	process_single_redirection(t_redir *redir)
{
	int	status;

	int saved_errno; // Variable to store errno immediately after failure
	// Check for missing filename (syntax error)
	if (!redir || !redir->file || redir->file[0] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: "
						"missing filename after redirection\n",
						STDERR_FILENO);
		g_exit_status = 2; // Syntax errors often have exit code 2 in bash
		return (-1);
	}
	status = 0; // Initialize status
	// --- Perform the specific redirection ---
	// These handle_* functions should return -1 on error and set errno.
	if (redir->type == T_REDIR_IN)
		status = handle_input_redir(redir->file);
	else if (redir->type == T_REDIR_OUT)
		status = handle_output_redir(redir->file);
	else if (redir->type == T_APPEND)
		status = handle_append_redir(redir->file);
	else if (redir->type == T_HEREDOC)
		status = handle_heredoc_redir(redir->file);
	// Add else if for other types if necessary
	// --- Check for errors and print appropriate message ---
	if (status < 0)
	{
		saved_errno = errno; // Save errno immediately after open/dup2 failure
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redir->file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		// Print the actual system error message
		ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		g_exit_status = 1; // Redirection errors typically yield exit code 1
		return (-1);       // Return error
	}
	return (0); // Success
}

/* Main function that processes all redirections in the list */
// moje, dziala tez z tymi 2 errory
int	setup_redirections(t_redir *redir_list)
{
	while (redir_list)
	{
		if (process_single_redirection(redir_list) < 0)
			return (-1);
		redir_list = redir_list->next;
	}
	return (0);
}
