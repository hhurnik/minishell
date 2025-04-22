/*It is used to mark variables and functions to be passed to child
processes. Basically, a variable will be included in child
process environments without affecting other environments. */

/*When a program is invoked it is given an array of strings called the environment. This is a list of name-value pairs,
	of the form name=value.

The shell provides several ways to manipulate the environment.
On invocation, the shell scans its own environment and creates
a parameter for each name found, automatically marking it for
export to child processes. Executed commands inherit the environment.
 The export and declare  -x commands allow parameters and functions
 to be added to and deleted from the environment. If the value of
 a parameter in the environment is modified, the new value becomes
 part of the environment, replacing the old. The environment
 inherited by any executed command consists of the shell's initial
 environment, whose values may be modified in the shell, less any
 pairs removed by the unset command,
	plus any additions via the export and declare -x commands.*/

/*The export command in a shell (like bash, zsh,
	etc.) is used to set environment variables
and make them available to child processes (like programs launched from the shell).*/

//     -------- without arguments it lists all environment variables that are marked -----------
// for export
// why use export? because by default, shell variables are local to the shell,
// when you export them, you make them part of the environment,
//	which is passed to child processes like ls, vim, gcc

#include "../inc/minishell.h"


/**
 * Calculate the length of an environment variable's value
 */
size_t env_len(t_env *env)
{
    if (!env || !env->value)
        return (0);
    return (ft_strlen(env->value));
}

/**
 * Calculate the length of an environment variable's key
 */
size_t env_key_len(t_env *env)
{
    if (!env || !env->key)
        return (0);
    return (ft_strlen(env->key));
}

/**
 * Print all environment variables in export format
 * Format: declare -x KEY="VALUE"
 */
static void print_env_export_format(t_env *env)
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

/**
 * Check if a variable name is valid for export
 * Valid names: start with letter or underscore, followed by letters, numbers, or underscores
 */
static int is_valid_identifier(char *name)
{
    int i;
    
    if (!name || !*name)
        return (0);
    
    // First character must be letter or underscore
    if (!ft_isalpha(name[0]) && name[0] != '_')
        return (0);
    
    // Remaining characters must be letters, digits or underscores
    i = 1;
    while (name[i] && name[i] != '=')
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return (0);
        i++;
    }
    
    return (1);
}

/**
 * Add or update an environment variable
 */
static int set_env_var(const char *var, t_env **env)
{
    char *key;
    char *value;
    char *equals;
    t_env *existing;
    
    // Find equals sign to separate key and value
    equals = ft_strchr(var, '=');
    if (!equals)
    {
        // No equals sign, just mark for export (already in env)
        key = ft_strdup(var);
        value = ft_strdup("");
    }
    else
    {
        // Extract key and value
        key = ft_substr(var, 0, equals - var);
        value = ft_strdup(equals + 1);
    }
    
    // Check for memory allocation errors
    if (!key || !value)
    {
        free(key);
        free(value);
        return (ERROR);
    }
    
    // Check if variable already exists
    existing = get_env(key, *env);
    if (existing)
    {
        // Update existing variable
        if (equals)  // Only update value if '=' was provided
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

/**
 * Export builtin command - with no options
 * If no arguments, print all environment variables in export format
 * Otherwise, set or update variables in the environment
 */

 int bi_export(char **args, t_env **env)
 {
	 int i;
	 int status;
	 
	 if (!env)
		 return (ERROR);
	 
	 // No arguments - print all variables in export format
	 if (!args[1])
	 {
		 print_env_export_format(*env);
		 return (SUCCESS);
	 }
	 
	 // With arguments - add or update variables
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

/**
 * Export with arguments (for your reference, implement later)
 */
int bi_export_with_args(char **args, t_env **env)
{
    int i;
    int status;
    
    if (!env)
        return (ERROR);
    
    // No arguments - print all variables in export format
    if (!args[1])
    {
        print_env_export_format(*env);
        return (SUCCESS);
    }
    
    // With arguments - add or update variables
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
