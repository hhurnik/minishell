#include "inc/minishell.h"


/**
 * Łączy trzy stringi w jeden (np. "dir", "/", "cmd" → "dir/cmd").
 * Zwraca nowy string lub NULL przy błędzie alokacji.
 */
char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char *tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	char *result = ft_strjoin(tmp, s3);
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
	if (!cmd || !env)
		return (NULL);

	// 1. Sprawdź, czy komenda jest już ścieżką bezwzględną/względną
	if (ft_strchr(cmd, '/'))
	{
		return ((access(cmd, X_OK) == 0) ? ft_strdup(cmd) : NULL);
	}

	// 2. Pobierz wartość zmiennej PATH
	char *path = get_env_value("PATH", env);
	if (!path)
		return (NULL);

	// 3. Podziel PATH na katalogi
	char **dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);

	// 4. Przeszukaj każdy katalog w PATH
	char *full_path = NULL;
	for (int i = 0; dirs[i]; i++)
	{
		const char *dir = dirs[i];
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
	}

	ft_free_split(dirs);
	return (NULL);
}

void	ft_free_split(char **split)
{
	if (!split)
		return ;

	for (int i = 0; split[i]; i++)
	{
		free(split[i]);
	}
	free(split);
}