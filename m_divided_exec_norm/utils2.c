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

/**
 * Szuka pełnej ścieżki do komendy w zmiennej środowiskowej PATH.
 * - cmd: nazwa komendy (np. "ls").
 * - env: lista zmiennych środowiskowych.
 * Zwraca: pełna ścieżka (malloc) lub NULL, jeśli nie znaleziono.
 */
char	*get_exec_path(char *cmd, t_env *env)
{
	char		*path;
	const char	*dir;
	char		*full_path;
	char		**dirs;
	int			i;

	i = 0;
	if (!cmd || !env)
		return (NULL);
	// 1. Sprawdź, czy komenda jest już ścieżką bezwzględną/względną
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	// 2. Pobierz wartość zmiennej PATH
	path = get_env_value("PATH", env);
	if (!path)
		return (NULL);
	// 3. Podziel PATH na katalogi
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	// 4. Przeszukaj każdy katalog w PATH
	full_path = NULL;
	while (dirs[i])
	{
		dir = dirs[i];
		if (dir[0] == '\0')
			dir = "."; // Obsługa pustych ścieżek (np. PATH=:/bin)
		full_path = ft_strjoin3(dir, "/", cmd);
		if (!full_path)
			continue ;
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(dirs);
			return (full_path);
		}
		free(full_path);
		full_path = NULL;
		i++;
	}
	ft_free_split(dirs);
	return (NULL);
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
