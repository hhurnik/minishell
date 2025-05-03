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

//function 1 deepsel
/////TO JEST DOBRA FUNKCJA, DZIALA DLA 3 HEREDOCOW, POWYZEJ TYLKO TESTUJE
// int process_single_redirection(t_redir *redir)
// {
//     int status = 0;
//     int saved_errno;

//     if (!redir || !redir->file || redir->file[0] == '\0') 
//     {
//         ft_putstr_fd("minishell: syntax error: missing filename or delimiter\n", STDERR_FILENO);
//         g_exit_status = 2;
//         return (-1);
//     }
//     if (redir->type == T_REDIR_IN)
//         status = handle_input_redir(redir->file);
//     else if (redir->type == T_REDIR_OUT)
//         status = handle_output_redir(redir->file);
//     else if (redir->type == T_APPEND)
//         status = handle_append_redir(redir->file);
//     else if (redir->type == T_HEREDOC)
//     {
//         if (redir->heredoc_fd < 0) 
//         {
//             ft_putstr_fd("minishell: internal error: invalid heredoc fd for ", STDERR_FILENO);
//              ft_putstr_fd(redir->file, STDERR_FILENO);
//              ft_putstr_fd("\n", STDERR_FILENO);
//             status = -1;
//         }
//         else 
//         {
//             if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0)
//                  status = -1;
//             close(redir->heredoc_fd);
//         }
//     }
//     if (status < 0)
//     {
//         saved_errno = errno;
//         ft_putstr_fd("minishell: ", STDERR_FILENO);
//         ft_putstr_fd(redir->file, STDERR_FILENO);
//         ft_putstr_fd(": ", STDERR_FILENO);
//         ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
//         ft_putstr_fd("\n", STDERR_FILENO);
//         g_exit_status = 1;
//         return (-1);
//     }
//     return (0);
// }

///////////////////podzial process_single_redirection//////////////////////

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

/* heredoc_utils.c */
int	handle_heredoc2(t_redir *redir)
{
	if (redir->heredoc_fd < 0)
	{
		ft_putstr_fd("minishell: heredoc error: ", STDERR_FILENO);
		ft_putstr_fd(redir->file, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (-1);
	}
	if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0)
		return (-1);
	close(redir->heredoc_fd);
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

/* error_utils.c */
void	print_redirection_error(t_redir *redir)
{
	int	saved_errno;

	saved_errno = errno;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(redir->file, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	g_exit_status = 1;
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




///////////////////koniec podzial process_single_redirection//////////////////////


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
