
#include "inc/minishell.h"

int	is_builtin(char *cmd)
{
	if ((!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd")
			|| !strcmp(cmd, "export") || !strcmp(cmd, "unset") || !strcmp(cmd,
				"env") || !strcmp(cmd, "exit")))
		return (1); // jesli to builtin
	else
		return (0);
}

int run_builtin(t_cmd *cmd, t_env **env)
{
    if (!strcmp(cmd->args[0], "echo"))
        return (bi_echo(cmd->args));
    if (!strcmp(cmd->args[0], "pwd"))
        return (bi_pwd(cmd->args));
    if (!strcmp(cmd->args[0], "exit"))
        return (bi_exit(cmd->args, *env));
    if (!strcmp(cmd->args[0], "unset"))
        return (bi_unset(cmd->args, env));
    if (!strcmp(cmd->args[0], "export"))
        return (bi_export(cmd->args, env));
    if (!strcmp(cmd->args[0], "env"))
        return (bi_env(cmd->args, env));
    if (!strcmp(cmd->args[0], "cd"))
        return (bi_cd(cmd->args, env));
    return (0); //not a builtin (shouldn't happen cause we check is_builtin first)
}
