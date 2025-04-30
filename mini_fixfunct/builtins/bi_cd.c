#include "../inc/minishell.h"

int	handle_tilde_path(char **path, char *arg, t_env **env, char *old_pwd)
{
	char	*home;

	home = get_env_value("HOME", *env);
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		free(old_pwd);
		return (ERROR);
	}
	*path = ft_strjoin(home, &arg[1]);
	if (!*path)
	{
		free(old_pwd);
		return (ERROR);
	}
	return (CONTINUE);
}

int	handle_regular_path(char **path, char *arg, char *old_pwd)
{
	*path = ft_strdup(arg);
	if (!*path)
	{
		free(old_pwd);
		return (ERROR);
	}
	return (CONTINUE);
}

int	process_path_argument(char **args, t_env **env, char **old_pwd)
{
	char	*path;
	int		result;

	if (args[1][0] == '~' && args[1][1] == '/')
		result = handle_tilde_path(&path, args[1], env, *old_pwd);
	else
		result = handle_regular_path(&path, args[1], *old_pwd);
	if (result != CONTINUE)
		return (result);
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		free(path);
		free(*old_pwd);
		return (ERROR);
	}
	free(path);
	return (update_pwds(env, *old_pwd));
}
//MOJE
// int	bi_cd(char **args, t_env **env)
// {
// 	char	*old_pwd;
// 	int		special_case_result;

// 	old_pwd = getcwd(NULL, 0);
// 	if (!old_pwd)
// 	{
// 		if (get_env_value("PWD", *env))
// 			old_pwd = ft_strdup(get_env_value("PWD", *env));
// 		else
// 			old_pwd = ft_strdup("");
// 	}
// 	if (!args[1])
// 		return (handle_no_args(env, &old_pwd));
// 	special_case_result = process_special_case(args[1], env, &old_pwd);
// 	if (special_case_result != CONTINUE)
// 	{
// 		free(old_pwd); //dodane valgrind
// 		return (special_case_result);
// 	}
// 	return (process_path_argument(args, env, &old_pwd));
// }

int bi_cd(char **args, t_env **env)
{
    char *old_pwd;
    int special_case_result;

    // ADD this check
    if (args[1] && args[2])
    {
         ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
         return (ERROR); // Return 1
    }

    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
	{
		if (get_env_value("PWD", *env))
			old_pwd = ft_strdup(get_env_value("PWD", *env));
		else
			old_pwd = ft_strdup("");
	}
	if (!args[1])
		return (handle_no_args(env, &old_pwd));
	special_case_result = process_special_case(args[1], env, &old_pwd);
	if (special_case_result != CONTINUE)
	{
		free(old_pwd); //dodane valgrind
		return (special_case_result);
	}
	return (process_path_argument(args, env, &old_pwd));
}






// //NOWE
// int	bi_cd(char **args, t_env **env)
// {
//     char	*old_pwd;
//     int		special_case_result;

//     old_pwd = getcwd(NULL, 0);
//     if (!old_pwd)
//     {
//         if (get_env_value("PWD", *env))
//             old_pwd = ft_strdup(get_env_value("PWD", *env));
//         else
//             old_pwd = ft_strdup("");
//         if (!old_pwd) // Handle strdup failure
//             return (ERROR);
//     }
//     if (!args[1])
//     {
//         int result = handle_no_args(env, &old_pwd);
//         free(old_pwd); // Free old_pwd after handling no args
//         return (result);
//     }
//     special_case_result = process_special_case(args[1], env, &old_pwd);
//     if (special_case_result != CONTINUE)
//     {
//         free(old_pwd); // Free old_pwd if special case fails
//         return (special_case_result);
//     }
//     return (process_path_argument(args, env, &old_pwd));
// }
