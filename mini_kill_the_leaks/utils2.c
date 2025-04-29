#include "inc/minishell.h"

/**
 * Łączy trzy stringi w jeden (np. "dir", "/", "cmd" → "dir/cmd").
 * Zwraca nowy string lub NULL przy błędzie alokacji.
 */
char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

void	ft_free_split(char **split)
{
	int	i;

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

/* Main function to find executable path */
//leaki
char	*get_exec_path(char *cmd, t_env *env)
{
	char	*path_value;
	char	**path_dirs;
	char	*exec_path;

	if (!cmd || !env)
		return (NULL);
	// First check if it's already an absolute/relative path
	exec_path = check_absolute_path(cmd);
	if (exec_path)
		return (exec_path);
	// Get PATH environment variable
	path_value = get_env_value("PATH", env);
	if (!path_value)
		return (NULL);
	// Split PATH into directories
	path_dirs = ft_split(path_value, ':');
	if (!path_dirs)
		return (NULL);
	// Search through each directory in PATH
	exec_path = search_in_path_dirs(cmd, path_dirs);
	ft_free_split(path_dirs);
	return (exec_path);
}
/* Helper function to search through PATH directories */
char	*search_in_path_dirs(char *cmd, char **dirs)
{
	int			i;
	char		*full_path;
	const char	*dir;

	i = 0;
	while (dirs[i])
	{
		dir = dirs[i];
		if (dir[0] == '\0')
			dir = "."; // Handle empty paths (e.g., PATH=:/bin)
		full_path = ft_strjoin3(dir, "/", cmd);
		if (!full_path)
			continue ;
		// w oryginale ponizej jeszcze w ifie ft_split_free
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
