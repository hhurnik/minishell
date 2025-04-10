#include "../inc/minishell.h"
#include <aio.h>
#include <stdarg.h>

// this below worsk

// char	*get_env_value2(const char *key, t_env *env)
// {
// 	while (env)
// 	{
// 		if (strcmp(env->key, key) == 0)
// 			return (env->value);
// 		env = env->next;
// 	}
// 	return (NULL);
// }

// int	update_pwds(t_env **env, char *old_pwd)
// {
// 	t_env *pwd_var = get_env("PWD", *env);
// 	t_env *oldpwd_var = get_env("OLDPWD", *env);

// 	// Update PWD variable
// 	if (pwd_var)
// 		free(pwd_var->value);
// 	pwd_var->value = getcwd(NULL, 0); // Get current directory

// 	// Update OLDPWD variable
// 	if (oldpwd_var)
// 		free(oldpwd_var->value);
// 	oldpwd_var->value = old_pwd; // Set old directory

// 	return (pwd_var && oldpwd_var ? SUCCESS : ERROR);
// }

// size_t	get_length_with_space(char *str, const char *cmd)
// {
// 	size_t i = 0;
// 	while (str[i] && strncmp(&str[i], cmd, strlen(cmd)) != 0)
// 		i++;
// 	return (i + strlen(cmd));
// }

// t_env	*get_env(const char *key, t_env *env)
// {
// 	while (env)
// 	{
// 		if (strcmp(env->key, key) == 0)
// 			return (env);
// 		env = env->next;
// 	}
// 	return (NULL);
// }

// static int	change_directory_and_update(t_env **env, char *new_pwd,
// 		char *old_pwd)
// {
// 	if (!new_pwd)
// 	{
// 		free(old_pwd);
// 		return (ERROR);
// 	}
// 	if (chdir(new_pwd) == ERROR)
// 	{
// 		free(new_pwd);
// 		free(old_pwd);
// 		return (ERROR);
// 	}
// 	free(new_pwd);
// 	return (update_pwds(env, old_pwd));
// }

// static int	handle_home_or_empty(t_env **env, char **old_pwd)
// {
// 	const char *home = get_env_value2("HOME", *env);
// 	if (!home)
// 		return (ERROR);
// 	*old_pwd = ft_strdup(get_env_value2("PWD", *env));
// 	return (chdir(home) == ERROR ? ERROR : SUCCESS);
// }

// static int	handle_dash_case(t_env **env, char **old_pwd)
// {
// 	const char *old_pwd_value = get_env_value2("OLDPWD", *env);
// 	if (!old_pwd_value)
// 		return (ERROR);
// 	*old_pwd = ft_strdup(get_env_value2("PWD", *env));
// 	return (chdir(old_pwd_value) == ERROR ? ERROR : SUCCESS);
// }

// static int	handle_dollar_case(char *comment, t_env **env, char **old_pwd)
// {
// 	t_env *var_entry = get_env(&comment[1], *env);
// 	if (!var_entry || chdir(var_entry->value) == ERROR)
// 	{
// 		free(*old_pwd);
// 		return (ERROR);
// 	}
// 	return (SUCCESS);
// }

// static int	process_special_case(char *arg, t_env **env, char **old_pwd)
// {
// 	if (arg[0] == '\0' || (arg[0] == '~' && (ft_isspace(arg[1])
// 				|| arg[1] == '\0')))
// 		return (handle_home_or_empty(env, old_pwd));
// 	if (arg[0] == '-')
// 		return (handle_dash_case(env, old_pwd));
// 	if (arg[0] == DOLLAR)
// 		return (handle_dollar_case(arg, env, old_pwd));
// 	return (ERROR);
// }

// static int	construct_and_change_directory(char *comment, t_env **env,
// 		char **old_pwd)
// {
// 	char *target_path = NULL;
// 	size_t i = get_length_with_space(comment, "cd");

// 	// Process special cases like ~, -, $ etc.
// 	if (process_special_case(&comment[i], env, old_pwd) == SUCCESS)
// 		return (SUCCESS);

// 	// Handle `cd ~` or `cd ~/folder`
// 	if (comment[i] == '~' && comment[i + 1] == '/')
// 	{
// 		const char *home = get_env_value2("HOME", *env);
// 		if (!home)
// 		{
// 			free(*old_pwd);
// 			return (ERROR_cd);
// 		}
// 		target_path = ft_strjoin(home, &comment[i + 1]);
// 	}
// 	else
// 	{
// 		target_path = ft_strdup(&comment[i]);
// 	}

// 	return (change_directory_and_update(env, target_path, *old_pwd));
// }

// int	bi_cd(char *comment, t_env **env)
// {
// 	char *old_pwd = getcwd(NULL, 0);
// 	if (!old_pwd)
// 		return (ERROR_cd);

// 	// Call function to construct the directory path and handle cd operation
// 	int result = construct_and_change_directory(comment, env, &old_pwd);
// 	if (result == ERROR_cd)
// 		free(old_pwd);

// 	return (result);
// }

////new - trying out better naming, still to test

char	*get_env_value(const char *key, t_env *env)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

t_env	*find_env_var(const char *key, t_env *env)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	update_pwd_vars(t_env **env, const char *old_pwd)
{
	t_env *pwd = find_env_var("PWD", *env);
	t_env *oldpwd = find_env_var("OLDPWD", *env);
	if (!pwd || !oldpwd)
		return (ERROR);

	free(pwd->value);
	free(oldpwd->value);
	pwd->value = getcwd(NULL, 0);
	oldpwd->value = (char *)old_pwd;
	return (pwd && oldpwd ? SUCCESS : ERROR);
}

size_t	skip_cd_cmd(char *str, const char *cmd)
{
	size_t idx = 0;
	while (str[idx] && strncmp(&str[idx], cmd, strlen(cmd)) != 0)
		idx++;
	return (idx + strlen(cmd));
}

int	change_directory_with_update(t_env **env, char *new_dir,
		const char *old_pwd)
{
	if (!new_dir || chdir(new_dir) == ERROR)
	{
		free(new_dir);
		return (ERROR);
	}
	return (update_pwd_vars(env, old_pwd));
}

int	handle_home_or_empty_dir(t_env **env, const char **old_pwd)
{
	const char *home = get_env_value("HOME", *env);
	if (!home)
		return (ERROR);

	*old_pwd = ft_strdup(get_env_value("PWD", *env));
	return (chdir(home) == ERROR ? ERROR : SUCCESS);
}

int	handle_dash_dir(t_env **env, const char **old_pwd)
{
	const char *oldpwd_value = get_env_value("OLDPWD", *env);
	if (!oldpwd_value)
		return (ERROR);

	*old_pwd = ft_strdup(get_env_value("PWD", *env));
	return (chdir(oldpwd_value) == ERROR ? ERROR : SUCCESS);
}

int	handle_var_dir(char *arg, t_env **env, const char **old_pwd)
{
	t_env *env_var = find_env_var(&arg[1], *env);
	if (!env_var || chdir(env_var->value) == ERROR)
	{
		free(*old_pwd);
		return (ERROR);
	}
	return (SUCCESS);
}

int	process_special_cd_cases(char *arg, t_env **env, const char **old_pwd)
{
	if (arg[0] == '\0' || (arg[0] == '~' && (ft_isspace(arg[1])
				|| arg[1] == '\0')))
		return (handle_home_or_empty_dir(env, old_pwd));
	if (arg[0] == '-')
		return (handle_dash_dir(env, old_pwd));
	if (arg[0] == DOLLAR)
		return (handle_var_dir(arg, env, old_pwd));
	return (ERROR);
}

int	build_and_change_directory(char *cmd, t_env **env, const char **old_pwd)
{
	size_t idx = skip_cd_cmd(cmd, "cd");
	char *target_path = NULL;

	if (process_special_cd_cases(&cmd[idx], env, old_pwd) == SUCCESS)
		return (SUCCESS);

	if (cmd[idx] == '~' && cmd[idx + 1] == '/')
	{
		const char *home = get_env_value("HOME", *env);
		if (!home)
			return (ERROR);
		target_path = ft_strjoin(home, &cmd[idx + 1]);
	}
	else
		target_path = ft_strdup(&cmd[idx]);

	return (change_directory_with_update(env, target_path, *old_pwd));
}

int	bi_cd(char *cmd, t_env **env)
{
	const char *old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (ERROR);

	int result = build_and_change_directory(cmd, env, &old_pwd);
	if (result == ERROR)
		free((char *)old_pwd);
	return result;
}

// koniec new