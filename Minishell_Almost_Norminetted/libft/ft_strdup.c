/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:01:09 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:35:08 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*dup;

	len = ft_strlen(s) + 1;
	i = 0;
	dup = malloc(sizeof(char) * len);
	if (dup == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
/*
Strdup takes a pointer to a constant char (input string) and
returns a pointer to a duplicated string.

1:We need variables to store lenght of the string, an iterator
and a pointer to duplicated string

2:Calculate the length of the input string using "strlen",
then adds 1 for the null terminator. This determines the amount
of memory needed for the duplicated string.

3: Use malloc to allocate memory for the duplicate string.
he length is len, which includes space for characters and null
terminator

4: Check if memory allocation was succesfull. If not, the
function immediatelly returns "NULL" to indicate error.

5: Next we have to copie each character from the original string
to the duplicated string using While loop.

6: Next line adds the null terminator to the end of the duplicated
string.

7: Returns the pointer to the duplicated string.
*/

/*
int	main(void)
{
	const char	*originalString = "Hello, World!";
	char		*duplicatedString;

	// Test ft_strdup function
	duplicatedString = ft_strdup(originalString);
	if (duplicatedString != NULL)
	{
		printf("Original String: %s\n", originalString);
		printf("Duplicated String: %s\n", duplicatedString);
		// Remember to free the allocated memory
		free(duplicatedString);
	}
	else
	{
		printf("Memory allocation failed.\n");
	}
	return (0);
}
*/
