/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:01:34 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:39:25 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i--;
	}
	return (NULL);
}
// 7. Finding length of string s

// 8. If c = '\0', we return pointer to the end of the string

// 12. Iterating backwards throught the string in search for last c occurence

// 16. Return NULL if c is nowhere to find

/*
int	main(void) {
	const char *str = "Hello, world!";
	int search_char = 'o';

	// Test 1: Search for the last occurrence of 'o' character in the string
	char *result = ft_strrchr(str, search_char);
	if (result != NULL) {
		printf("Last occurrence of character '%c' found at position: %ld\n",
			search_char, result - str);
	} else {
		printf("Character '%c' not found.\n", search_char);
	}

	// Test 2: Search for the last occurrence of 'x' character in the string
	search_char = 'x';
	result = ft_strrchr(str, search_char);
	if (result != NULL) {
		printf("Last occurrence of character '%c' found at position: %ld\n",
			search_char, result - str);
	} else {
		printf("Character '%c' not found.\n", search_char);
	}

	// Test 3: Search for the last occurrence of '\0' character (null terminator)
	search_char = '\0';
	result = ft_strrchr(str, search_char);
	if (result != NULL) {
		printf("Last occurrence of character '\\0' found at position: %ld\n",
			result - str);
	} else {
		printf("Character '\\0' not found.\n");
	}

	return (0);
}
*/
