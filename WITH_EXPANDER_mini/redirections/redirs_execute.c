#include "../inc/minishell.h"

// Redirection core functionality
// int validate_redirection(t_redir *redir);
// int handle_redirection_type(t_redir *redir);
// int process_single_redirection(t_redir *redir);
// int setup_redirections(t_redir *redir_list);
// void restore_std_fds(int stdin_copy, int stdout_copy);

void	restore_std_fds(int stdin_copy, int stdout_copy)
{
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

int	validate_redirection(t_redir *redir)
{
	if (!redir)
	{
		ft_putstr_fd("minishell: syntax error\n", STDERR_FILENO);
		g_exit_status = 2;
		return (-1);
	}
	if (!redir->file || redir->file[0] == '\0')
	{
		ft_putstr_fd("minishell: missing filename\n", STDERR_FILENO);
		g_exit_status = 2;
		return (-1);
	}
	return (0);
}

/* redirection_utils.c */
int	handle_redirection_type(t_redir *redir)
{
	if (redir->type == T_REDIR_IN)
		return (handle_input_redir(redir->file));
	else if (redir->type == T_REDIR_OUT)
		return (handle_output_redir(redir->file));
	else if (redir->type == T_APPEND)
		return (handle_append_redir(redir->file));
	else if (redir->type == T_HEREDOC)
		return (handle_heredoc2(redir));
	return (0);
}

/* process_redirection.c */
int	process_single_redirection(t_redir *redir)
{
	int	status;

	if (validate_redirection(redir) < 0)
		return (-1);
	status = handle_redirection_type(redir);
	if (status < 0)
	{
		print_redirection_error(redir);
		return (-1);
	}
	return (0);
}

// potrojny heredoc dziala
/* Main function that processes all redirections in the list */
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
