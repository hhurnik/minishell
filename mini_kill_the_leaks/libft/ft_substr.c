/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 18:58:26 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/29 16:51:59 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;
	size_t	j;

	i = start;
	j = 0;
	sub = (char *)malloc(len + 1);
	if (!s || !sub)
		return (NULL);
	while (i < ft_strlen(s) && j < len)
		sub[j++] = s[i++];
	sub[j] = '\0';
	return (sub);
}
/*
int	main(void)
{
	const char		*originalString = "Hello, World!";
	unsigned int	start;
	size_t			length;
	char			*substring;

	start = 7;
	length = 5;
	// Test ft_substr function
	substring = ft_substr(originalString, start, length);
	if (substring != NULL)
	{
		printf("Original String: %s\n", originalString);
		printf("Substring: %s\n", substring);
		// Remember to free the allocated memory
		free(substring);
	}
	else
	{
		printf("Memory allocation or index out of bounds.\n");
	}
	return (0);
}
*/
/*
Description :
 Allocates (with malloc(3)) and returns a substring from the string ’s’.
The substring begins at index ’start’ and is of maximum size ’len’.

*/
