#include "inc/minishell.h"

int	is_special_var(char *str, int i)
{
	return (str[i] == '$' && str[i + 1] == '?');
}

int	is_braced_var(char *str, int i)
{
	return (str[i] == '$' && str[i + 1] == '{');
}

int	is_standard_var(char *str, int i)
{
	return (str[i] == '$');
}

void	append_char(char **result, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	*result = ft_strjoin_free(*result, tmp);
}

int	handle_special_var(char **result, int i)
{
	*result = ft_strjoin_free(*result, ft_itoa(g_exit_status));
	return (i + 2);
}