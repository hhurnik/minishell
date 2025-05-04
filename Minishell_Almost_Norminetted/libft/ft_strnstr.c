/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:01:30 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:38:54 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (little[0] == '\0')
		return ((char *)big);
	while (big[i] != '\0' && i < len)
	{
		while (big[i + j] == little[j] && i + j < len)
		{
			if (little[j + 1] == '\0')
				return ((char *)&big[i]);
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}
/*
int	main(void) {
	// Test 1: Search for "world" in "Hello, world!" with len = 13
	const char *str1 = "Hello, world!";
	const char *substr1 = "world";
	size_t len1 = 13;
	printf("Test 1:\n");
	char *result1 = ft_strnstr(str1, substr1, len1);
	if (result1 != NULL) {
		printf("Result: \"%s\"\n", result1);
	} else {
		printf("Substring not found.\n");
	}

	// Test 2: Search for "world" in "Hello, world!" with len = 6
	const char *str2 = "Hello, world!";
	const char *substr2 = "world";
	size_t len2 = 6;
	printf("\nTest 2:\n");
	char *result2 = ft_strnstr(str2, substr2, len2);
	if (result2 != NULL) {
		printf("Result: \"%s\"\n", result2);
	} else {
		printf("Substring not found.\n");
	}

	// Test 3: Search for "world" in "Hello, world!" with len = 5
	const char *str3 = "Hello, world!";
	const char *substr3 = "world";
	size_t len3 = 5;
	printf("\nTest 3:\n");
	char *result3 = ft_strnstr(str3, substr3, len3);
	if (result3 != NULL) {
		printf("Result: \"%s\"\n", result3);
	} else {
		printf("Substring not found.\n");
	}

	return (0);
}
*/



