#include "../inc/minishell.h"

t_env *env_new(const char *key, const char *value)
{
    t_env *new_var;
    
    new_var = (t_env *)malloc(sizeof(t_env));
    if (!new_var)
        return (NULL);
    
    new_var->key = ft_strdup(key);
    if (!new_var->key)
    {
        free(new_var);
        return (NULL);
    }
    
    if (value)
        new_var->value = ft_strdup(value);
    else
        new_var->value = ft_strdup("");
    
    if (!new_var->value)
    {
        free(new_var->key);
        free(new_var);
        return (NULL);
    }
    
    new_var->next = NULL;
    return (new_var);
}

t_env	*get_env(const char *key, t_env *env)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*get_env_value5(const char *key, t_env *env)
{
	t_env	*var;

	var = get_env(key, env);
	if (!var)
		return (NULL);
	return (var->value);
}

int	update_pwds(t_env **env, char *old_pwd)
{
	t_env	*pwd_var;
	t_env	*oldpwd_var;
	char	*new_pwd;

	pwd_var = get_env("PWD", *env);
	oldpwd_var = get_env("OLDPWD", *env);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("minishell: cd");
		free(old_pwd);
		return (ERROR);
	}
	// Update or create PWD
	if (pwd_var)
	{
		free(pwd_var->value);
		pwd_var->value = new_pwd;
	}
	else
		env_add_back(env, env_new("PWD", new_pwd));
	// Update or create OLDPWD
	if (oldpwd_var)
	{
		free(oldpwd_var->value);
		oldpwd_var->value = old_pwd;
	}
	else
		env_add_back(env, env_new("OLDPWD", old_pwd));
	return (SUCCESS);
}

int	handle_home_case(t_env **env, char **old_pwd)
{
	char	*home;

	home = get_env_value5("HOME", *env);
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		return (ERROR);
	}
	*old_pwd = get_env_value5("PWD", *env);
	if (!*old_pwd)
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
	char	*oldpwd;

	oldpwd = get_env_value5("OLDPWD", *env);
	if (!oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		return (ERROR);
	}
	*old_pwd = get_env_value5("PWD", *env);
	if (!*old_pwd)
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
	if (arg[0] == '$')
	{
		t_env	*var;
		char	*var_name = &arg[1];
		
		var = get_env(var_name, *env);
		if (!var || !var->value)
		{
			ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
			ft_putstr_fd(var_name, STDERR_FILENO);
			ft_putstr_fd(" not set\n", STDERR_FILENO);
			return (ERROR);
		}
		*old_pwd = getcwd(NULL, 0);
		if (chdir(var->value) == -1)
		{
			perror("minishell: cd");
			free(*old_pwd);
			return (ERROR);
		}
		return (SUCCESS);
	}
	return (CONTINUE);
}

int			bi_cd(char **args, t_env **env)
{
	char	*path;
	char	*old_pwd;
	int		special_case_result;

	if (args[1] && args[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO), ERROR);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		old_pwd = ft_strdup(get_env_value5("PWD", *env));
	// Handle special cases (~, -, $VAR)
	if (args[1])
	{
		special_case_result = process_special_case(args[1], env, &old_pwd);
		if (special_case_result != CONTINUE)
			return (special_case_result == SUCCESS ? update_pwds(env, old_pwd) : ERROR);
	}
	// Handle regular path
	path = args[1];
	if (!path) // cd without arguments
		return (handle_home_case(env, &old_pwd) == SUCCESS ? update_pwds(env, old_pwd) : ERROR);
	// Handle ~/path case
	if (path[0] == '~' && path[1] == '/')
	{
		char	*home = get_env_value5("HOME", *env);
		if (!home)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO), free(old_pwd), ERROR);
		path = ft_strjoin(home, &path[1]);
	}
	else
		path = ft_strdup(path);
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		free(path);
		free(old_pwd);
		return (ERROR);
	}
	free(path);
	return (update_pwds(env, old_pwd));
}