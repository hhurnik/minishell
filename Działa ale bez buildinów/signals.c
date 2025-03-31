#include "inc/minishell.h"

volatile sig_atomic_t g_signal_flag = 0;

void handle_sigint(int sig) {
    (void)sig;
    g_signal_flag = 1;
    ft_putstr_fd("\nminishell$ ", STDOUT_FILENO);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void setup_signals(void) {
    struct sigaction sa_int = {.sa_handler = handle_sigint};
    sigaction(SIGINT, &sa_int, NULL);
    signal(SIGQUIT, SIG_IGN); // Ctrl-\ nic nie robi
}