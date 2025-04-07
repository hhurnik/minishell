
// deletes the variables during program execution
//- both functions and shell variables - only shell variables

// if a variable is readonly you cannot unset it

// search internal environment variable list, remove the matching entry

#include "../inc/minishell.h"


// Compare a variable name with the key in a t_env structure
int	env_cmpr(t_env *env, char *varname)
{
	if (!env || !env->key || !varname) // Check for NULL pointers
		return (0);

	// Compare the key in t_env with varname
	return (ft_strncmp(env->key, varname, ft_strlen(varname)) == 0);
}

// Remove an environment variable from the list
void	remove_env_var(char *varname, t_env **env)
{
	int i = 0;

	while (env[i])
	{
		// Use env_cmpr to compare the variable name
		if (env_cmpr(env[i], varname))
		{
			free(env[i]->key);
			free(env[i]->value);
			free(env[i]);

			// Shift all remaining entries up
			while (env[i + 1])
			{
				env[i] = env[i + 1];
				i++;
			}
			env[i] = NULL;
			break ; // Only one instance possible, so break
		}
		i++;
	}
}

// Updated bi_unset function to return an int (g_state)
int	bi_unset(char **argv, t_env **env)
{
	int i = 1;
	int g_state = 0; // Initialize g_state to success (0)

	if (!argv || !env) // Check for NULL pointers
		return (1);    // Return failure status

	while (argv[i])
	{
		// Optional: validate variable name here (skip invalid names)
		remove_env_var(argv[i], env);
		i++;
	}

	return (g_state); // Return the status
}