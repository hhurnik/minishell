// without arguments it lists all environment variables that are marked
// why use export? because by default, shell variables are local to the shell,
// when you export them, you make them part of the environment,
//	which is passed to child processes like ls, vim, gcc

#include "../inc/minishell.h"

// calculate the length of an environment variable's value
size_t	env_len(t_env *env)
{
	if (!env || !env->value)
		return (0);
	return (ft_strlen(env->value));
}

// print all environment variables in export format
// // format: declare -x KEY="VALUE"
void	print_env_export_format(t_env *env)
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

//diala, leakami
int	parse_env_var(const char *var, char **key, char **value)
{
	char	*equals;

	equals = ft_strchr(var, '=');
	if (!equals)
	{
		*key = ft_strdup(var);
		*value = ft_strdup("");
	}
	else
	{
		*key = ft_substr(var, 0, equals - var);
		*value = ft_strdup(equals + 1);
	}
	if (!*key || !*value)
	{
		free(*key);
		free(*value);
		return (ERROR);
	}
	return (SUCCESS);
}


/* Function 2: Update or add the environment variable to the list */
//diala,  leakami
//moje
int	set_env_var(const char *var, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*existing;

	if (parse_env_var(var, &key, &value) == ERROR)
	{
		free(key); //dodane valgrind
		free(value); //dodane valgrind
		return (ERROR);
	}
	existing = get_env(key, *env);
	if (existing)
	{
		if (ft_strchr(var, '='))
			// Only update value if there was an equals sign
		{
			free(existing->value);
			existing->value = value;
		}
		free(key);
	}
	else
	{
		env_add_back(env, env_new(key, value));
		free(key);
		free(value);
	}
	return (SUCCESS);
}

// int	set_env_var(const char *var, t_env **env)
// {
// 	char	*key = NULL;    // Initialize to NULL
// 	char	*value = NULL;   // Initialize to NULL
// 	t_env	*existing;

// 	// 1. Parsing
// 	if (parse_env_var(var, &key, &value) == ERROR)
// 	{
// 		// Assuming parse_env_var doesn't guarantee freeing on error
// 		// free(key); // Safely free potential partial allocations
// 		// free(value); // free(NULL) is safe
// 		// If parse_env_var *does* clean up itself, these frees are not needed.
// 		return (ERROR);
// 	}

// 	// 2. Find existing variable
// 	existing = get_env(key, *env);

// 	// 3. Handle Existing Variable
// 	if (existing)
// 	{
// 		// Check if '=' was present in the original argument 'var'
// 		if (ft_strchr(var, '='))
// 		{
// 			free(existing->value);  // Free the old value in the node
// 			existing->value = value; // Assign the NEW value (ownership transferred)
// 			value = NULL; // Prevent freeing the transferred value below
// 		}
// 		// else: No '=' sign, do not change existing->value

// 		free(key);   // Free the parsed key (always done for existing vars)
// 		key = NULL;
// 		free(value); // Free the parsed value IF it wasn't transferred (free(NULL) is safe)
// 		value = NULL;
// 	}
// 	// 4. Handle New Variable
// 	else
// 	{
// 		// Assuming env_new COPIES key and value
// 		t_env *new_node = env_new(key, value);
// 		if (!new_node) // Handle allocation failure in env_new
// 		{
// 			free(key);   // Must free if node creation failed
// 			free(value);
// 			return (ERROR);
// 		}
// 		env_add_back(env, new_node);

// 		// Since env_new copied, we free the originals allocated by parse_env_var
// 		free(key);
// 		key = NULL;
// 		free(value);
// 		value = NULL;
// 	}

// 	return (SUCCESS);
// }

//moje
int	bi_export(char **args, t_env **env)
{
	int	i;
	int	status;

	if (!env)
		return (ERROR);
	if (!args[1])
	{
		print_env_export_format(*env);
		return (SUCCESS);
	}
	status = SUCCESS;
	i = 0;
	while (args[++i] && (status == SUCCESS || 1))
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = ERROR;
		}
		else if (set_env_var(args[i], env) != SUCCESS)
			status = ERROR;
	}
	return (status);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*new;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (n < len)
		len = n;
	new = (char *)malloc(len + 1);
	if (!new)
		return (NULL);
	ft_memcpy(new, s, len);
	new[len] = '\0';
	return (new);
}



// // //tego uywam w wieksosci
int is_valid_identifier(const char *arg)
{
	char	*name;
	char	*equal_pos;
	int		i;

	if (!arg || !*arg)
		return (0);

	// Get the position of '=' if present
	equal_pos = ft_strchr(arg, '=');

	// Extract the variable name (up to '=' or full string if no '=')
	if (equal_pos)
		name = ft_strndup(arg, equal_pos - arg);
	else
		name = ft_strdup(arg);

	if (!name)
		return (0);

	// Check first character: must be alpha or underscore
	if (!ft_isalpha(name[0]) && name[0] != '_')
	{
		free(name);
		return (0);
	}

	// Check remaining characters
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			free(name);
			return (0);
		}
		i++;
	}

	free(name);
	return (1);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isalpha, isalnum



// Placeholder functions (replace with your actual implementations)
//t_env *get_env(const char *name, t_env *env_list) { /* ... find node ... */ (void)name; (void)env_list; return NULL; }
//t_env *env_new(char *key, char *value) { /* ... create node, taking ownership ... */ t_env *n = malloc(sizeof(*n)); if(n){n->key=key; n->value=value; n->next=NULL;} return n; }
//void env_add_back(t_env **env_list, t_env *new_node) { /* ... add node ... */ (void)env_list; (void)new_node; }
//void print_env_export_format(t_env *env_list) { /* ... print sorted env ... */ (void)env_list; printf("declare -x EXAMPLE=\"value\"\n");}
// Simple ft_putstr_fd for error messages
//void ft_putstr_fd(const char *s, int fd) { write(fd, s, strlen(s)); }
// Simple ft_strdup (you likely have a better one in libft)
//char *ft_strdup(const char *s) { if (!s) return NULL; char *d = malloc(strlen(s) + 1); if(d) strcpy(d, s); return d; }
// Simple ft_strndup (you likely have a better one in libft)
// char *ft_strndup(const char *s, size_t n) {
//     if (!s) return NULL;
//     size_t len = strnlen(s, n);
//     char *d = malloc(len + 1);
//     if (!d) return NULL;
//     memcpy(d, s, len);
//     d[len] = '\0';
//     return d;
// }
// --- End of assumed functions ---
/////////////////same, debuugowanie

// /**
//  * @brief Checks if a string is a valid shell identifier.
//  *        Starts with '_' or letter, followed by '_' or alphanum.
//  * @param name The string to check.
//  * @return 1 if valid, 0 otherwise.
//  */
// int is_valid_identifier(char *name)
// {
// 	// // --- DEBUG PRINT ---
// 	// fprintf(stderr, "DEBUG: is_valid_identifier received: '%s'\n", name ? name : "(null)");
// 	// // --- END DEBUG ---

// 	if (!name || name[0] == '\0') // Cannot be NULL or empty
// 	{
// 		// --- DEBUG PRINT ---
// 		//fprintf(stderr, "DEBUG: is_valid_identifier failed: NULL or empty\n");
// 		// --- END DEBUG ---
// 		return (0);
// 	}

// 	// First character: must be alpha or underscore
// 	if (!isalpha((unsigned char)name[0]) && name[0] != '_')
// 	{
// 		// --- DEBUG PRINT ---
// 		//fprintf(stderr, "DEBUG: is_valid_identifier failed: Invalid start char '%c' (code %d)\n", name[0], name[0]);
// 		// --- END DEBUG ---
// 		return (0);
// 	}

// 	// Remaining characters: must be alphanum or underscore
// 	// Use size_t for loop index
// 	for (size_t i = 1; name[i]; i++)
// 	{
// 		if (!isalnum((unsigned char)name[i]) && name[i] != '_')
// 		{
// 			// --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: is_valid_identifier failed: Invalid char '%c' (code %d) at index %zu\n", name[i], name[i], i);
// 			// --- END DEBUG ---
// 			return (0);
// 		}
// 	}

// 	// --- DEBUG PRINT ---
// 	//fprintf(stderr, "DEBUG: is_valid_identifier passed for: '%s'\n", name);
// 	// --- END DEBUG ---
// 	return (1);
// }

/////////////////////DEBUGGOWANIE INNA WERSJA FUKCJI
// /**
//  * @brief Implements the export builtin command.
//  *
//  * @param args Null-terminated array of arguments (args[0] is "export").
//  * @param env Pointer to the environment list head pointer.
//  * @return SUCCESS (0) or ERROR (1).
//  */
// int bi_export(char **args, t_env **env)
// {
// 	int i;
// 	int overall_status = SUCCESS; // Renamed
// 	char *arg;
// 	char *equal_sign;
// 	char *key = NULL;
// 	char *value = NULL;
// 	t_env *existing_var;

// 	// --- DEBUG PRINT ---
// 	//fprintf(stderr, "DEBUG: Entering bi_export\n");
// 	// --- END DEBUG ---

// 	if (!env) {
// 		fprintf(stderr, "DEBUG: bi_export received NULL env pointer\n"); // Should not happen
// 		return (ERROR);
// 	}

// 	// Case 1: `export` with no arguments
// 	if (args[1] == NULL)
// 	{
// 		// --- DEBUG PRINT ---
// 		//fprintf(stderr, "DEBUG: bi_export called with no arguments, printing env\n");
// 		// --- END DEBUG ---
// 		print_env_export_format(*env);
// 		return (SUCCESS);
// 	}

// 	// Case 2: `export` with arguments
// 	i = 1;
// 	while ((arg = args[i]) != NULL) // Check args[i] before incrementing i
// 	{
// 		// --- DEBUG PRINT ---
// 		//fprintf(stderr, "DEBUG: Processing arg[%d]: '%s'\n", i, arg);
// 		// --- END DEBUG ---
// 		equal_sign = strchr(arg, '=');
// 		key = NULL;
// 		value = NULL;

// 		// 1. Parse Key and Value
// 		if (equal_sign != NULL)
// 		{
// 			// Argument has form "KEY=VALUE"
// 			size_t key_len = equal_sign - arg;
// 			// --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: Found '=' at index %ld. Attempting to strndup %zu chars for key.\n", (long)(equal_sign - arg), key_len);
// 			// --- END DEBUG ---
// 			key = ft_strndup(arg, key_len);
// 			if (key) // Check key allocation before allocating value
// 			{
// 				// --- DEBUG PRINT ---
// 				//fprintf(stderr, "DEBUG: Key allocated: '%s'. Attempting to strdup value from '%s'\n", key, equal_sign + 1);
// 				// --- END DEBUG ---
// 				value = ft_strdup(equal_sign + 1);
// 			}

//             if (!key || !value) { // Check both allocations after attempts
//                 fprintf(stderr, "DEBUG: Allocation failed! key=%p, value=%p\n", (void*)key, (void*)value);
//                 free(key); // Safe to free NULL
//                 free(value); // Safe to free NULL
// 				ft_putstr_fd("minishell: export: memory allocation error\n", 2);
//                 overall_status = ERROR;
//                 i++; // Increment i to move to next arg in the loop condition
//                 continue;
//             }
// 			// --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: Parsed KEY='%s', VALUE='%s'\n", key, value);
// 			// --- END DEBUG ---
// 		}
// 		else
// 		{
// 			// Argument has form "KEY"
// 			// --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: No '=' found. Attempting to strdup arg for key.\n");
// 			// --- END DEBUG ---
// 			key = ft_strdup(arg);
//             if (!key) {
//                 fprintf(stderr, "DEBUG: Allocation failed for key!\n");
// 				ft_putstr_fd("minishell: export: memory allocation error\n", 2);
//                 overall_status = ERROR;
//                 i++; // Increment i
//                 continue;
//             }
// 			// value remains NULL
// 			// --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: Parsed KEY='%s', VALUE=(null)\n", key);
// 			// --- END DEBUG ---
// 		}

// 		// 2. Validate Key (ONLY the key part)
// 		// --- DEBUG PRINT ---
// 		//fprintf(stderr, "DEBUG: Calling is_valid_identifier for key: '%s'\n", key);
// 		// --- END DEBUG ---
// 		if (!is_valid_identifier(key)) // Pass the parsed key
// 		{
// 			// --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: is_valid_identifier returned false.\n");
// 			// --- END DEBUG ---
// 			ft_putstr_fd("minishell: export: `", 2);
// 			ft_putstr_fd(arg, 2); // Print the original invalid argument
// 			ft_putstr_fd("': not a valid identifier\n", 2);
// 			free(key);
// 			free(value); // Free value too (might be NULL)
// 			overall_status = ERROR; // Mark command status as failed
//             i++; // Increment i
// 			continue;       // Move to the next argument
// 		}
// 		// --- DEBUG PRINT ---
// 		//fprintf(stderr, "DEBUG: is_valid_identifier returned true.\n");
// 		// --- END DEBUG ---


// 		// 3. Find and Update/Add Environment Variable
// 		// --- DEBUG PRINT ---
// 		//fprintf(stderr, "DEBUG: Looking for existing var with key: '%s'\n", key);
// 		// --- END DEBUG ---
// 		existing_var = get_env(key, *env);

// 		if (existing_var)
// 		{
// 			// --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: Found existing variable for key '%s'. Old value: '%s'\n", key, existing_var->value ? existing_var->value : "(null)");
// 			// --- END DEBUG ---
// 			if (equal_sign != NULL) // Only update value if '=' was present in arg
// 			{
// 				// --- DEBUG PRINT ---
// 				//fprintf(stderr, "DEBUG: Updating value to: '%s'\n", value);
// 				// --- END DEBUG ---
// 				free(existing_var->value); // Free the OLD value in the node
// 				existing_var->value = value; // Assign the NEW value (ownership transferred)
//                 value = NULL; // Prevent freeing below
// 			}
//             else {
//                 // If no '=', the parsed value (which should be NULL) is not needed
//                 // --- DEBUG PRINT ---
// 				//fprintf(stderr, "DEBUG: No '=' in arg, not updating existing value. Freeing parsed value (if any).\n");
// 				// --- END DEBUG ---
//                 free(value);
//                 value = NULL;
//             }
//             // Key was only needed for lookup, free the parsed key
//             // --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: Freeing parsed key '%s' (lookup only).\n", key);
// 			// --- END DEBUG ---
//             free(key);
//             key = NULL;
// 		}
// 		else
// 		{
// 			// Variable does not exist, add a new one
// 			// --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: No existing variable found. Creating new node with KEY='%s', VALUE='%s'\n", key, value ? value : "(null)");
// 			// --- END DEBUG ---

//             // Assumes env_new duplicates key/value because it takes const char*
//             // If env_new took ownership (non-const char*), we wouldn't free key/value here.
// 			t_env *new_node = env_new(key, value); // Pass pointers from parsing
//             if (!new_node) {
//                 fprintf(stderr, "DEBUG: env_new failed!\n");
//                 free(key);
//                 free(value);
// 				ft_putstr_fd("minishell: export: memory allocation error\n", 2);
//                 overall_status = ERROR;
//                 i++; // Increment i
//                 continue;
//             }
// 			// --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: Adding new node %p to list.\n", (void*)new_node);
// 			// --- END DEBUG ---
// 			env_add_back(env, new_node);

//             // Since env_new (presumably) duplicated the const char* args,
//             // we MUST free the key/value obtained from ft_strdup/ft_strndup
//             // --- DEBUG PRINT ---
// 			//fprintf(stderr, "DEBUG: Freeing parsed key '%s' and value '%s' after duplication by env_new.\n", key, value ? value : "(null)");
// 			// --- END DEBUG ---
//             free(key);
//             key = NULL;
//             free(value);
//             value = NULL;
// 		}
//         // Defensive frees (should be NULL if logic above is correct)
//         free(key);
//         free(value);

//         i++; // Increment argument index AFTER processing
// 	} // End while loop through arguments

// 	// --- DEBUG PRINT ---
// 	//fprintf(stderr, "DEBUG: Exiting bi_export with status %d\n", overall_status);
// 	// --- END DEBUG ---
// 	return (overall_status);
// }