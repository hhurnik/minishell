#include "../inc/minishell.h"

void	restore_std_fds(int stdin_copy, int stdout_copy)
{
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}

int	process_single_redirection(t_redir *redir)
{
	int	status;

	if (!redir->file || redir->file[0] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: "
			"missing filename after redirection\n", STDERR_FILENO);
		return (-1);
	}
	status = 0;
	if (redir->type == T_REDIR_IN)
		status = handle_input_redir(redir->file);
	else if (redir->type == T_REDIR_OUT)
		status = handle_output_redir(redir->file);
	else if (redir->type == T_APPEND)
		status = handle_append_redir(redir->file);
	else if (redir->type == T_HEREDOC)
		status = handle_heredoc_redir(redir->file);
	if (status < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redir->file, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (-1);
	}
	return (0);
}

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
