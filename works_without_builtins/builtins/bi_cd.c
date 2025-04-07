#include "../inc/minishell.h"

//lapie env variable by key, if it doesnt exisrt returns NULL
static const char	*get_env_value(const char *key, t_env *env)
{
	t_env *entry = get_env(key, env);
	if (entry != NULL)
		return (entry->value);
	return (NULL);
}

//if a path is valid, zmienia directory to the given path
//updates PWD/OLDPWD if successs, zwalnia old on failure
static int	change_to_path(const char *path, t_env **env, char *old_pwd)
{
	if (path == NULL)
	{
		free(old_pwd);
		return (ERROR);
	}
	if (chdir(path) == ERROR)
	{
		free(old_pwd);
		return (ERROR);
	}
	return (update_pwds(env, old_pwd));
}

//changes directory to the given path (relative or absolute)
//failure-> frees both `old_pwd` and path, prints an error
//success--> updates PWD/OLDPWD and frees path
static int	change_directory(char *path, t_env **env, char *old_pwd)
{
	if (chdir(path) == ERROR)
	{
		free(old_pwd);
		free(path);
		return (print_error(__FILE__, __LINE__, 1));
	}
	free(path);
	return (update_pwds(env, old_pwd));
}

//handles cd -     -> xmienia do poprzedniego WD (OLDPWD).
//error if OLDPWD is not set or chdir fails
static int	handle_oldpwd_case(t_env **env, char *old_pwd)
{
	const char *oldpwd = get_env_value("OLDPWD", *env);
	if (oldpwd == NULL)
		return (ERROR);
	if (chdir(oldpwd) == ERROR)
		return (ERROR);
	return (update_pwds(env, old_pwd));
}

//handles cd $VAR -> changes dir to the value of the given variable
//var_name shouldnt have the leading $
static int	handle_variable_path(char *var_name, t_env **env, char *old_pwd)
{
	const char *path = get_env_value(var_name, *env);
	if (path == NULL)
		return (ERROR);
	if (chdir(path) == ERROR)
		return (ERROR);
	return (update_pwds(env, old_pwd));
}



//special cd cases cd or cd ~ -> HOME, cd -  -> OLDPWD
//cd $VAR -> value of VAR
//SUCCESS if a special path was handled -- or error if not
static int	handle_special_paths(char *arg, t_env **env, char *old_pwd)
{
	if (arg[0] == '\0')
		return (change_to_path(get_env_value("HOME", *env), env, old_pwd));

	if (arg[0] == '~' && (ft_isspace(arg[1]) || arg[1] == '\0'))
		return (change_to_path(get_env_value("HOME", *env), env, old_pwd));

	if (arg[0] == '-' && arg[1] == '\0')
		return (handle_oldpwd_case(env, old_pwd));

	if (arg[0] == '$')
		return handle_variable_path(&arg[1], env, old_pwd);

	return ERROR;
}

//arg parsing, finds the path (handles ~, $, -) and calls chdir
//updates env vars if the dir changed succesfully
int	bi_cd(char *input, t_env **env)
{
	char *target_path;
	char *old_pwd;
	size_t cmd_offset;
	char *arg;

	cmd_offset = get_length_with_space(input, "cd");
	arg = &input[cmd_offset];

	old_pwd = getcwd(NULL, 0);
	if (old_pwd == NULL)
		return ERROR;

	if (handle_special_paths(arg, env, old_pwd) == SUCCESS)
		return SUCCESS;

	if (arg[0] == '~' && arg[1] == '/')
	{
		const char *home = get_env_value("HOME", *env);
		if (home == NULL)
		{
			free(old_pwd);
			return ERROR;
		}
		target_path = ft_strjoin(home, &arg[1]);
	}
	else
	{
		target_path = ft_strdup(arg);
	}

	if (target_path == NULL)
	{
		free(old_pwd);
		return ERROR;
	}

	return change_directory(target_path, env, old_pwd);
}

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