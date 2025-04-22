#include "inc/minishell.h"

//"Volatile" forces the compiler to reload the variable every time it's accessed.

volatile sig_atomic_t g_signal_flag = 0;
volatile sig_atomic_t g_in_command = 0;

void handle_sigint(int sig)
{
    (void)sig;
    g_signal_flag = 1;
    
    if (g_in_command) {
        // If we're running a command, just print newline
        write(STDOUT_FILENO, "\n", 1);
    } else {
        // If we're at the prompt, redisplay it
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void	setup_signals(void)
{
	struct sigaction sa_int = {.sa_handler = handle_sigint};
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN); // Ctrl-\ nic nie robi
}
