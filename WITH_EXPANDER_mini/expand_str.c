#include "inc/minishell.h"


static char	*finalize_expansion(char *result, bool error_occurred);
static int	append_char_to_result(char **result_ptr, char current_char);
static int	process_char_type(char **res, char *str, int i, t_env *env);
static bool	run_expansion_loop(char **res, char *str, int *i_ptr, t_env *env);

static int	append_char_to_result(char **result_ptr, char current_char)
{
	char	*current_result;
	char	*new_result;

	current_result = *result_ptr;
	new_result = ft_strjoin_char(current_result, current_char);
	if (!new_result)
	{
		perror("minishell: malloc error in ft_strjoin_char");
		// Do not free current_result here, the main loop handles cleanup
		return (0); // Signal failure
	}
	//free(current_result);
	*result_ptr = new_result;
	return (1); // Signal success
}

static int	process_char_type(char **res, char *str, int i, t_env *env)
{
	int	next_i;

	next_i = -2; // Default invalid value
	if (is_special_var(str, i))
	{
		next_i = handle_special_var(res, i);
		// handle_special_var returns next index or -1 on error
	}
	else if (is_braced_var(str, i))
	{
		next_i = handle_braced_var(res, str, i, env);
		// handle_braced_var returns next index or -1 on error
	}
	else if (is_standard_var(str, i))
	{
		next_i = handle_standard_var(res, str, i, env);
		// handle_standard_var returns next index or -1 on error
	}
	else
	{
		if (!append_char_to_result(res, str[i]))
			next_i = -1; // Signal error from append helper
		else
			next_i = i + 1; // Move to the next character
	}
	return (next_i); // Return next index or -1
}


static bool	run_expansion_loop(char **res, char *str, int *i_ptr, t_env *env)
{
	int	current_i;
	int	next_i;

	current_i = *i_ptr;
	next_i = process_char_type(res, str, current_i, env);
	if (next_i < 0)
	{
		return (true); // Error occurred
	}
	else
	{
		*i_ptr = next_i; // Update index for the next iteration
		return (false); // No error this iteration
	}
}

static char	*finalize_expansion(char *result, bool error_occurred)
{
	if (error_occurred)
	{
		if (result)
		{
			free(result);
		}
		return (NULL);
	}
	return (result);
}

char	*expand_str(char *str, t_env *env)
{
	char	*result;
	int		i;
	bool	error_occurred;

	// Initialize variables
	result = NULL;
	i = 0;
	error_occurred = false;

	// Initial allocation for the result string
	result = ft_strdup("");
	if (!result)
	{
		perror("minishell: malloc error in expand_str initial allocation");
		return (NULL); // Early exit on initial allocation failure
	}

	// Main loop to process the string
	while (str && str[i] && !error_occurred)
	{
		// Process one character/variable and check for errors
		error_occurred = run_expansion_loop(&result, str, &i, env);
		// Index 'i' is updated by run_expansion_loop on success
	}

	// Clean up and return the final result or NULL
	return (finalize_expansion(result, error_occurred));
}
