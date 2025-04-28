/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:30:40 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:36:36 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	result = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (s1[i])
		result[j++] = s1[i++];
	i = 0;
	while (s2[i])
	{
		result[j] = s2[i];
		j++;
		i++;
	}
	result[j] = '\0';
	return (result);
}
/*
int	main(void)
{
	const char	*str1 = "Hello, ";
	const char	*str2 = "world!";
	char		*result;

	result = ft_strjoin(str1, str2);
	if (result != NULL)
	{
		printf("Result: %s\n", result);
		free(result); // Remember to free the allocated memory
	}
	else
	{
		printf("Memory allocation failed.\n");
	}
	return (0);
}
*/
/*
Parameters:
s1: The prefix string.
s2: The suffix string.

Return value:
The new string.
NULL if the allocation fails.

External functs.:
malloc

Description:
Allocates (with malloc(3)) and returns a newstring, which is the result of
the concatenation of ’s1’ and ’s2’.


1: Allocates memory for the concatenated string. The allocated size is
the sum of the lengths of s1 and s2, plus 1 for the null terminator.

2: Checks if both input strings are NULL. If so, returns NULL, indicating
that there's nothing to concatenate.

3: Checks if the memory allocation was successful. If not,
returns NULL to indicate a failure.

4: Copies characters from s1 to the result string until the null terminator
of s1 is encountered. Increments i after each copy.

5: Appends characters from s2 to the result string, starting where s1 left off.
Increments both i and j after each copy.

6: Adds a null terminator at the end of the concatenated string to ensure it is
properly terminated.

7: Returns the pointer to the concatenated string (result).
*/
