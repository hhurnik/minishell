// without arguments it lists all environment variables that are marked
// why use export? because by default, shell variables are local to the shell,
// when you export them, you make them part of the environment,
//	which is passed to child processes like ls, vim, gcc

#include "../inc/minishell.h"



//calculate the length of an environment variable's value

size_t env_len(t_env *env)
{
    if (!env || !env->value)
        return (0);
    return (ft_strlen(env->value));
}

//calculate the length of an environment variable's key

size_t env_key_len(t_env *env)
{
    if (!env || !env->key)
        return (0);
    return (ft_strlen(env->key));
}

//print all environment variables in export format
//format: declare -x KEY="VALUE"
void print_env_export_format(t_env *env)
{
    while (env)
    {
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putstr_fd(env->key, STDOUT_FILENO);
        ft_putstr_fd("=\"", STDOUT_FILENO);
        ft_putstr_fd(env->value, STDOUT_FILENO);
        ft_putstr_fd("\"\n", STDOUT_FILENO);
        env = env->next;
    }
}

//Add or update an environment variable
int set_env_var(const char *var, t_env **env)
{
    char *key;
    char *value;
    char *equals;
    t_env *existing;
    
    // Find "=" to separate key and value
    equals = ft_strchr(var, '=');
    if (!equals)
    {
        //nooo equals sign, just mark for export (already in env)
        key = ft_strdup(var);
        value = ft_strdup("");
    }
    else
    {
        // extract key and value
        key = ft_substr(var, 0, equals - var);
        value = ft_strdup(equals + 1);
    }
    
    if (!key || !value)
    {
        free(key);
        free(value);
        return (ERROR);
    }
    
    // check if variable already exists
    existing = get_env(key, *env);
    if (existing)
    {
        // update existing variable
        if (equals)  // only update value if '=' was given
        {
            free(existing->value);
            existing->value = value;
        }
        free(key);
    }
    else
    {
        // Add new variable
        env_add_back(env, env_new(key, value));
        free(key);
        free(value);
    }
    
    return (SUCCESS);
}



//export with no options
//If no arguments, print all env variables in export format
//otherwise, set or update variables in the environment
int bi_export(char **args, t_env **env)
 {
	 int i;
	 int status;
	 
	 if (!env)
		 return (ERROR);
	 
	 // no arguments - print all variables in export format
	 if (!args[1])
	 {
		 print_env_export_format(*env);
		 return (SUCCESS);
	 }
	 
	 // with arguments - add or update variables
	 status = SUCCESS;
	 i = 1;
	 while (args[i])
	 {
		 if (!is_valid_identifier(args[i]))
		 {
			 ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			 ft_putstr_fd(args[i], STDERR_FILENO);
			 ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			 status = ERROR;
		 }
		 else
		 {
			 if (set_env_var(args[i], env) != SUCCESS)
				 status = ERROR;
		 }
		 i++;
	 }
	 
	 return (status);
 }

