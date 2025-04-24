/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:01:05 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:34:41 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}
/*
int	main(void) {
	const char *str = "Hello, world!";
	int search_char = 'o';

	// Test 1: Search for 'o' character in the string
	char *result = ft_strchr(str, search_char);
	if (result != NULL) {
		printf("Character '%c' found at position: %ld\n", search_char, result
			- str);
	} else {
		printf("Character '%c' not found.\n", search_char);
	}

	// Test 2: Search for 'x' character in the string
	search_char = 'x';
	result = ft_strchr(str, search_char);
	if (result != NULL) {
		printf("Character '%c' found at position: %ld\n", search_char, result
			- str);
	} else {
		printf("Character '%c' not found.\n", search_char);
	}

	// Test 3: Search for '\0' character (null terminator)
	search_char = '\0';
	result = ft_strchr(str, search_char);
	if (result != NULL) {
		printf("Character '\\0' found at position: %ld\n", result - str);
	} else {
		printf("Character '\\0' not found.\n");
	}

	return (0);
}
*/
