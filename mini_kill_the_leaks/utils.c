#include "inc/minishell.h"

// Sprawdza, czy znak jest białym znakiem
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v');
}

// Łączy dwa stringi, zwalniając s1
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

// Dodaje pojedynczy znak do stringa
char	*ft_strjoin_char(char *s, char c)
{
	char	*new;
	int		len;

	len = 0;
	if (s)
		len = ft_strlen(s);
	new = malloc(len + 2); // +1 for char, +1 for '\0'
	if (!new)
		return (NULL);
	if (s)
		ft_memcpy(new, s, len);
	new[len] = c;
	new[len + 1] = '\0';
	free(s);
	return (new);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
