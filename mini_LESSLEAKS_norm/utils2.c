#include "inc/minishell.h"


/**
 * Łączy trzy stringi w jeden (np. "dir", "/", "cmd" → "dir/cmd").
 * Zwraca nowy string lub NULL przy błędzie alokacji.
 */
char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char *result;
	char *tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

void	ft_free_split(char **split)
{
	int i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

/* Helper function to check if command is an existing executable path */
char	*check_absolute_path(char *cmd)
{
	if (!cmd)
		return (NULL);
	// Check if command contains a slash (absolute/relative path)
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*get_exec_path(char *cmd, t_env *env)
{
	char *path_value;
	char **path_dirs;
	char *exec_path;
	struct stat statbuf; // Declare statbuf here for the explicit path check

	// Basic validation
	if (!cmd || cmd[0] == '\0') // Also check for empty command string
		return (NULL);
	if (!env)          // Need env to check PATH
		return (NULL); // Or decide how to handle no env

	// --- Check if cmd is an explicit path (contains '/') ---
	if (ft_strchr(cmd, '/'))
	{
		// Use stat() to check if the path exists. We don't check permissions here.
		// Let execve handle permissions, directory execution errors etc.
		if (stat(cmd, &statbuf) == 0)
		{
			// Path exists (it could be a file, directory, etc.)
			return (ft_strdup(cmd)); // Return a copy of the path
		}
		else
		{
			// Stat failed (errno is likely ENOENT - No such file or directory)
			return (NULL); // Indicate failure to find the explicit path
		}
	}

	// --- If not an explicit path, search in PATH ---
	path_value = get_env_value("PATH", env);
	if (!path_value || path_value[0] == '\0')
	{
		// PATH not set or empty, cannot search
		return (NULL);
	}

	path_dirs = ft_split(path_value, ':');
	if (!path_dirs)
	{
		// ft_split failed (malloc error likely)
		// Optionally: perror("minishell: ft_split failed for PATH");
		return (NULL);
	}

	// Search through each directory listed in PATH
	exec_path = search_in_path_dirs(cmd, path_dirs);
		// This might return NULL or a malloc'd path

	// Clean up the allocated path directory array
	ft_free_split(path_dirs);

	// Return the result (NULL if not found in PATH,
	//	or the allocated full path)
	return (exec_path);
}

// //moje
char	*search_in_path_dirs(char *cmd, char **dirs)
{
	int i;
	char *full_path;
	const char *dir;

	i = 0;
	while (dirs[i])
	{
		dir = dirs[i];
		if (dir[0] == '\0')
			dir = "."; // Handle empty paths (e.g., PATH=:/bin)
		full_path = ft_strjoin3(dir, "/", cmd);
		if (!full_path)
			continue ;

		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}




//nowy proba cleanup 2.05
void cleanup_all_resources(t_resources *res, int *fds_to_close, int fd_count)
{
    int i;

    // Clean up file descriptors
    for (i = 0; i < fd_count; i++)
    {
        if (fds_to_close[i] >= 0)
            close(fds_to_close[i]);
    }

    // Clean up allocated memory
    if (res)
    {
        if (res->input)
            free(res->input);
        if (res->tokens)
            free_tokens(res->tokens);
        if (res->cmds)
            free_cmds(res->cmds);
        if (res->env)
            free_env(res->env);
    }

    // Clear readline history
    rl_clear_history();
}