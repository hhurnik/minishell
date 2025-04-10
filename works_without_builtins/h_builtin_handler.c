
#include "inc/minishell.h"

int	is_builtin(char *cmd)
{
	// !strcmp(cmd, "cd") is true if cmd is "cd".
	if ((!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd")
			|| !strcmp(cmd, "export") || !strcmp(cmd, "unset") || !strcmp(cmd,
				"env") || !strcmp(cmd, "exit")))
		return (1); // jesli to builtin
	else
		return (0);
}

int	execute_builtin(char **args, t_env **env)
{
	// char **env; //skads to wiać - global? cy prekazana jako argument?
	if (is_builtin(args[0]) == 1)
	{
		if (!strcmp(args[0], "echo"))
			return (bi_echo(args));
		// if (!strcmp(args[0], "cd"))
		// 	return (bi_cd(args[1], env));
		if (!strcmp(args[0], "pwd"))
			return (bi_pwd(args));
		if (!strcmp(args[0], "env"))
			return (bi_env(args));
		if (!strcmp(args[0], "exit"))
			return (bi_exit(args));
		if (!strcmp(args[0], "unset"))
			return (bi_unset(args, env)); // change to int
		if (!strcmp(args[0], "export"))
			return (bi_export(env)); // environment ////change to int
	}
	return (0); // jesli nie jest to builtin
}

int	run_builtin(t_cmd *cmd, t_env **env)
{
	if (!strcmp(cmd->args[0], "echo"))
		return (bi_echo(cmd->args));
	if (!strcmp(cmd->args[0], "pwd"))
		return (bi_pwd(cmd->args));
	if (!strcmp(cmd->args[0], "exit"))
		return (bi_exit(cmd->args));
	if (!strcmp(cmd->args[0], "unset"))
		return (bi_unset(cmd->args, env));
	if (!strcmp(cmd->args[0], "export"))
		return (bi_export(env));
	if (!strcmp(cmd->args[0], "env"))
		return (bi_env(cmd->args));
	return (0);
		// Not a builtin (shouldn't happen since we check is_builtin first)
}

/////krotsa version

// int execute_builtin(char **args)
// {
//     if (!strcmp(args[0], "echo"))
//         return (ft_echo(args));
//     else if (!strcmp(args[0], "cd"))
//         return (ft_cd(args));
//     else if (!strcmp(args[0], "pwd"))
//         return (ft_pwd(args));
//     else if (!strcmp(args[0], "export"))
//         return (ft_export(args));
//     else if (!strcmp(args[0], "unset"))
//         return (ft_unset(args));
//     else if (!strcmp(args[0], "env"))
//         return (ft_env(args));
//     else if (!strcmp(args[0], "exit"))
//         return (ft_exit(args));
//     return (0);
// }