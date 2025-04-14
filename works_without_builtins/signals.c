#include "inc/minishell.h"

//is volatile only for multi-threaded programs?
//why here?
//"Volatile" forces the compiler to reload the variable every time it's accessed.
// g_signal_flag is a global variable used to indicate a signal (like SIGINT) was received
// It's volatile and sig_atomic_t so it's safe to modify inside a signal handler

volatile sig_atomic_t	g_signal_flag = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_flag = 1; // a signal was received
	//ft_putstr_fd("\nminishell$ ", STDOUT_FILENO); //wypisz nowego prompta
	write(STDOUT_FILENO, "\n", 1); //zamiast powyzej, zeby nie wyswietlalo minishll 2x
	rl_on_new_line();	//move readline to a new line
	rl_replace_line("", 0); //clear the input line
	rl_redisplay(); //show the updated prompt
}

void	setup_signals(void)
{
	struct sigaction sa_int = {.sa_handler = handle_sigint};
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN); // Ctrl-\ nic nie robi
}