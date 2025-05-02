#include "../inc/minishell.h"

void	update_or_create_pwd(t_env **env, t_env *pwd_var, char *new_pwd)
{
	if (pwd_var)
	{
		free(pwd_var->value);
		pwd_var->value = new_pwd;
	}
	else
	{
		env_add_back(env, env_new("PWD", new_pwd));
		free(new_pwd);
	}
}

void	update_or_create_oldpwd(t_env **env, t_env *oldpwd_var,
		char *old_pwd_copy)
{
	if (oldpwd_var)
	{
		free(oldpwd_var->value);
		if (old_pwd_copy)
			oldpwd_var->value = old_pwd_copy;
		else
			oldpwd_var->value = ft_strdup("");
	}
	else if (old_pwd_copy)
	{
		env_add_back(env, env_new("OLDPWD", old_pwd_copy));
		free(old_pwd_copy);
	}
}

int	update_pwds(t_env **env, char *old_pwd)
{
	t_env	*pwd_var;
	t_env	*oldpwd_var;
	char	*new_pwd;
	char	*old_pwd_copy;

	old_pwd_copy = NULL;
	// Make a copy of old_pwd for OLDPWD
	if (old_pwd)
		old_pwd_copy = ft_strdup(old_pwd);
	pwd_var = get_env("PWD", *env);
	oldpwd_var = get_env("OLDPWD", *env);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("minishell: cd");
		free(old_pwd);
		free(old_pwd_copy);
		return (ERROR);
	}
	update_or_create_pwd(env, pwd_var, new_pwd);
	update_or_create_oldpwd(env, oldpwd_var, old_pwd_copy);
	// Always free the original old_pwd
	free(old_pwd);
	return (SUCCESS);
}
