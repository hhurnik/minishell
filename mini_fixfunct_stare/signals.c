#include "inc/minishell.h"

void	handle_sigint_prompt(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130; // Set exit status to 130 for Ctrl-C
}

void	handle_sigint_command(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	g_exit_status = 130; // Set exit status to 130 for Ctrl-C
}

void	setup_signals_for_prompt(void)
{
	signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_for_command(void)
{
	signal(SIGINT, handle_sigint_command);
	signal(SIGQUIT, SIG_IGN);
}
