#include "../inc/minishell.h"

int	handle_home_case(t_env **env, char **old_pwd)
{
	char	*home;
	char	*pwd_value;

	home = get_env_value("HOME", *env);
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		return (ERROR);
	}
	// Make a copy of the string instead of storing the direct pointer
	pwd_value = get_env_value("PWD", *env);
	if (pwd_value)
		*old_pwd = ft_strdup(pwd_value);
	else
		*old_pwd = getcwd(NULL, 0);
	if (chdir(home) == -1)
	{
		perror("minishell: cd");
		free(*old_pwd);
		return (ERROR);
	}
	return (SUCCESS);
}

int	handle_dash_case(t_env **env, char **old_pwd)
{
	char	*pwd_value;
	char	*oldpwd;

	oldpwd = get_env_value("OLDPWD", *env);
	if (!oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		return (ERROR);
	}
	// Make a copy of the string
	pwd_value = get_env_value("PWD", *env);
	if (pwd_value)
		*old_pwd = ft_strdup(pwd_value);
	else
		*old_pwd = getcwd(NULL, 0);
	if (chdir(oldpwd) == -1)
	{
		perror("minishell: cd");
		free(*old_pwd);
		return (ERROR);
	}
	ft_putendl_fd(oldpwd, STDOUT_FILENO);
	return (SUCCESS);
}

int	process_special_case(char *arg, t_env **env, char **old_pwd)
{
	if (!arg[0] || (arg[0] == '~' && (ft_isspace(arg[1]) || arg[1] == '\0')))
		return (handle_home_case(env, old_pwd));
	if (arg[0] == '-')
		return (handle_dash_case(env, old_pwd));
	return (CONTINUE);
}

int	handle_no_args(t_env **env, char **old_pwd)
{
	if (handle_home_case(env, old_pwd) == SUCCESS)
		return (update_pwds(env, *old_pwd));
	return (ERROR);
}
