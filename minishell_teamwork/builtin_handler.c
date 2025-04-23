
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

int	execute_builtin(char **args, t_env **env)
{
	if (is_builtin(args[0]) == 1)
	{
		if (!strcmp(args[0], "echo"))
			return (bi_echo(args));
		if (!strcmp(args[0], "cd"))
			return (bi_cd(&args[1], env));
		if (!strcmp(args[0], "pwd"))
			return (bi_pwd(args));
		if (!strcmp(args[0], "env"))
			return (bi_env(args, env));
		if (!strcmp(args[0], "exit"))
			return (bi_exit(args, *env));
		if (!strcmp(args[0], "unset"))
			return (bi_unset(args, env));
		if (!strcmp(args[0], "export"))
			return (bi_export(args, env));
	}
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
    {
        int status = bi_cd(cmd->args, env);
        update_pwd_env(env);  // After cd, update PWD environment variable
        return status;
    }
    return (0); // Not a builtin (shouldn't happen since we check is_builtin first)
}
