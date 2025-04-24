#include "../inc/minishell.h"

int	update_pwds(t_env **env, char *old_pwd)
{
    t_env	*pwd_var;
    t_env	*oldpwd_var;
    char *new_pwd;
    char *old_pwd_copy = NULL;

    // Make a copy of old_pwd for OLDPWD
    if (old_pwd)
        old_pwd_copy = ft_strdup(old_pwd);

    pwd_var = get_env("PWD", *env);
    oldpwd_var = get_env("OLDPWD", *env);
    new_pwd = getcwd(NULL, 0);
    
    if (!new_pwd) {
        perror("minishell: cd");
        free(old_pwd);
        free(old_pwd_copy);
        return (ERROR);
    }
    
    // Update or create PWD
    if (pwd_var) {
        free(pwd_var->value);
        pwd_var->value = new_pwd;
    } else {
        env_add_back(env, env_new("PWD", new_pwd));
        free(new_pwd);
    }
    
    // Update or create OLDPWD
    if (oldpwd_var) {
        free(oldpwd_var->value);
        oldpwd_var->value = old_pwd_copy ? old_pwd_copy : ft_strdup("");
    } else if (old_pwd_copy) {
        env_add_back(env, env_new("OLDPWD", old_pwd_copy));
        free(old_pwd_copy);
    }
    
    // Always free the original old_pwd
    free(old_pwd);
    return (SUCCESS);
}

int handle_home_case(t_env **env, char **old_pwd)
{
    char *home;

    home = get_env_value("HOME", *env);
    if (!home)
    {
        ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
        return (ERROR);
    }
    
    // Make a copy of the string instead of storing the direct pointer
    char *pwd_value = get_env_value("PWD", *env);
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

int handle_dash_case(t_env **env, char **old_pwd)
{
    char *oldpwd;

    oldpwd = get_env_value("OLDPWD", *env);
    if (!oldpwd)
    {
        ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
        return (ERROR);
    }
    
    // Make a copy of the string
    char *pwd_value = get_env_value("PWD", *env);
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
	if (arg[0] == '$')
	{
		t_env *var;
		char *var_name = &arg[1];

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

int	bi_cd(char **args, t_env **env)
{
    char *path = NULL;  // Initialize to NULL
    char *old_pwd;
    int special_case_result;

    // Initialize old_pwd safely
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd) {
        char *pwd_val = get_env_value("PWD", *env);
        if (pwd_val)
            old_pwd = ft_strdup(pwd_val);
        else
            old_pwd = ft_strdup("");  // Fallback to empty string
    }

    // Handle special cases
    if (args[1]) {
        special_case_result = process_special_case(args[1], env, &old_pwd);
        if (special_case_result != CONTINUE)
            return (special_case_result == SUCCESS ? update_pwds(env, old_pwd) : ERROR);
        
        // If we get here, we need to handle regular path
        if (args[1][0] == '~' && args[1][1] == '/') {
            char *home = get_env_value("HOME", *env);
            if (!home) {
                ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
                free(old_pwd);
                return ERROR;
            }
            path = ft_strjoin(home, &args[1][1]);
            if (!path) {
                free(old_pwd);
                return ERROR;
            }
        } else {
            path = ft_strdup(args[1]);
            if (!path) {
                free(old_pwd);
                return ERROR;
            }
        }
    } else {
        // Handle "cd" with no args
        return (handle_home_case(env, &old_pwd) == SUCCESS ? update_pwds(env, old_pwd) : ERROR);
    }

    // At this point, path is always allocated and needs to be freed
    if (chdir(path) == -1) {
        perror("minishell: cd");
        free(path);
        free(old_pwd);
        return ERROR;
    }

    free(path);  // Free path before returning
    return update_pwds(env, old_pwd);  // update_pwds takes ownership of old_pwd
}