/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:44 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:29:56 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	cc;
	size_t			i;

	str = (unsigned char *)s;
	cc = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (str[i] == cc)
			return ((void *)s + i);
		i++;
	}
	return (NULL);
}
/*
int	main(void) {
	char str[] = "Hello, world!";
	char *result;

	// Search for 'o' character in str
	result = ft_memchr(str, 'o', strlen(str));

	if (result != NULL) {
		printf("Character 'o' found at position: %ld\n", result - str);
	} else {
		printf("Character 'o' not found.\n");
	}

	// Search for 'x' character in str
	result = ft_memchr(str, 'x', strlen(str));

	if (result != NULL) {
		printf("Character 'x' found at position: %ld\n", result - str);
	} else {
		printf("Character 'x' not found.\n");
	}

	return (0);
}

*/
