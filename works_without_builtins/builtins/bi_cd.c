#include "../inc/minishell.h"
#include <aio.h>
#include <stdarg.h>

///stare
// t_env *get_env(char *key, t_env *env)
// {
//     // Iterate through the linked list of environment variables
//     while (env)
//     {
//         // Compare the key with the current environment variable's key
//         if (ft_strcmp(env->key, key) == 0)
//             return env;  // Return the entire t_env struct if the key matches
//         env = env->next;  // Move to the next environment variable
//     }
//     return NULL;  // Return NULL if no matching key is found
// }

// //lapie env variable by key, if it doesnt exisrt returns NULL
// char	*get_env_value2(char *key, t_env *env)
// {
// 	t_env *entry = get_env(key, env);
// 	if (entry != NULL)
// 		return (entry->value);
// 	return (NULL);
// }

// //if a path is valid, zmienia directory to the given path
// //updates PWD/OLDPWD if successs, zwalnia old on failure
// static int	change_to_path(const char *path, t_env **env, char *old_pwd)
// {
// 	if (path == NULL)
// 	{
// 		free(old_pwd);
// 		return (ERROR);
// 	}
// 	if (chdir(path) == ERROR)
// 	{
// 		free(old_pwd);
// 		return (ERROR);
// 	}
// 	// If chdir is successful, update PWD environment variable
//     update_pwd_env(env);  // This updates the PWD environment variable

//     // Optionally, you might want to update the OLDPWD environment variable here as well:
//     env_add_back(env, env_new("OLDPWD", ft_strdup(old_pwd))); // Add OLDPWD

//     free(old_pwd);  // Free the old_pwd after updating

//     return SUCCESS;  // Return success if everything is fine
// }


// int print_error(const char *file, int line, int return_code, ...)
// {
//     va_list args;

//     // Print the file name and line number where the error occurred
//     fprintf(stderr, "Error in file %s at line %d: ", file, line);

//     // Print additional error details if provided
//     va_start(args, return_code);
//     const char *message = va_arg(args, const char *);
//     if (message != NULL)
//     {
//         fprintf(stderr, "%s\n", message);
//     }
//     va_end(args);

//     return return_code;
// }



// //changes directory to the given path (relative or absolute)
// //failure-> frees both `old_pwd` and path, prints an error
// //success--> updates PWD/OLDPWD and frees path
// static int	change_directory(char *path, t_env **env, char *old_pwd)
// {
// 	if (chdir(path) == ERROR)
// 	{
// 		free(old_pwd);
// 		free(path);
// 		return (print_error(__FILE__, __LINE__, 1));
// 	}
// 	free(path);
// 	return (update_pwds(env, old_pwd));
// }

// //handles cd -     -> xmienia do poprzedniego WD (OLDPWD).
// //error if OLDPWD is not set or chdir fails
// static int	handle_oldpwd_case(t_env **env, char *old_pwd)
// {
// 	const char *oldpwd = get_env_value2("OLDPWD", *env);
// 	if (oldpwd == NULL)
// 		return (ERROR);
// 	if (chdir(oldpwd) == ERROR)
// 		return (ERROR);
// 	return (update_pwds(env, old_pwd));
// }

// //handles cd $VAR -> changes dir to the value of the given variable
// //var_name shouldnt have the leading $
// static int	handle_variable_path(char *var_name, t_env **env, char *old_pwd)
// {
// 	const char *path = get_env_value2(var_name, *env);
// 	if (path == NULL)
// 		return (ERROR);
// 	if (chdir(path) == ERROR)
// 		return (ERROR);
// 	return (update_pwds(env, old_pwd));
// }



// //special cd cases cd or cd ~ -> HOME, cd -  -> OLDPWD
// //cd $VAR -> value of VAR
// //SUCCESS if a special path was handled -- or error if not
// static int	handle_special_paths(char *arg, t_env **env, char *old_pwd)
// {
// 	if (arg[0] == '\0')
// 		return (change_to_path(get_env_value2("HOME", *env), env, old_pwd));

// 	if (arg[0] == '~' && (ft_isspace(arg[1]) || arg[1] == '\0'))
// 		return (change_to_path(get_env_value2("HOME", *env), env, old_pwd));

// 	if (arg[0] == '-' && arg[1] == '\0')
// 		return (handle_oldpwd_case(env, old_pwd));

// 	if (arg[0] == '$')
// 		return handle_variable_path(&arg[1], env, old_pwd);

// 	return ERROR;
// }

// //arg parsing, finds the path (handles ~, $, -) and calls chdir
// //updates env vars if the dir changed succesfully
// int	bi_cd(char *input, t_env **env)
// {
// 	char *target_path;
// 	char *old_pwd;
// 	size_t cmd_offset;
// 	char *arg;

// 	cmd_offset = get_length_with_space(input, "cd");
// 	arg = &input[cmd_offset];

// 	old_pwd = getcwd(NULL, 0);
// 	if (old_pwd == NULL)
// 		return ERROR;

// 	if (handle_special_paths(arg, env, old_pwd) == SUCCESS)
// 		return SUCCESS;

// 	if (arg[0] == '~' && arg[1] == '/')
// 	{
// 		const char *home = get_env_value2("HOME", *env);
// 		if (home == NULL)
// 		{
// 			free(old_pwd);
// 			return ERROR;
// 		}
// 		target_path = ft_strjoin(home, &arg[1]);
// 	}
// 	else
// 	{
// 		target_path = ft_strdup(arg);
// 	}

// 	if (target_path == NULL)
// 	{
// 		free(old_pwd);
// 		return ERROR;
// 	}

// 	return change_directory(target_path, env, old_pwd);
// }

// cd with only a relative or absolute path

// cd myFolder - relative path
// cd ../ - relative path (up one directory)
// cd /home/user - absolute path


// // test
// int main(int argc, char *argv[])
// {
//     //before changing
//     char *cwd = getcwd(NULL, 0);
//     if (cwd != NULL)
//     {
//         printf("Before cd: %s\n", cwd);
//         free(cwd);
//     }

//     int result = bi_cd(argv);

//     //afer
//     cwd = getcwd(NULL, 0);
//     if (cwd != NULL)
//     {
//         printf("After cd: %s\n", cwd);
//         free(cwd);
//     }

//     return (result);
// }

// ///// stare - obsluguje brak argumentu, my nie powinnismy

// //jesli samo cd - go to the home directory
// 