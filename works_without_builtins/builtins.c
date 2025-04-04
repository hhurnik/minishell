#include "inc/minishell.h"

// Przykład: cd
int ft_cd(char **args, t_env **env) {
    if (!args[1]) {
        // Zmień na $HOME
        char *home = get_env_value("HOME", *env);
        if (!home) {
            ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
            return 1;
        }
        chdir(home);
    } else {
        if (chdir(args[1]) != 0) {
            perror("minishell: cd");
            return 1;
        }
    }
    // Zaktualizuj PWD i OLDPWD
    update_pwd_env(env);
    return 0;
}

// Lista wbudowanych komend
int is_builtin(char *cmd) 
{
    return (ft_strcmp(cmd, "cd") == 0 ||
           ft_strcmp(cmd, "echo") == 0 ||
           ft_strcmp(cmd, "exit") == 0);
}
