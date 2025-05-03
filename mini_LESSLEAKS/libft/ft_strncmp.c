/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:01:27 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:38:26 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (!n)
		return (0);
	while ((*(s1 + i) == *(s2 + i)))
	{
		if (i == (n - 1) || *(s1 + i) == '\0' || *(s2 + i) == '\0')
			break ;
		i++;
	}
	return ((unsigned char)*(s1 + i) - (unsigned char)*(s2 + i));
}
/*
int	main(void) {
	// Test 1: Compare two strings with n = 5
	const char *str1 = "Hello";
	const char *str2 = "Hella";
	size_t n1 = 5;
	printf("Test 1:\n");
	int result1 = ft_strncmp(str1, str2, n1);
	printf("Result: %d\n", result1);

	// Test 2: Compare two strings with n = 3
	const char *str3 = "apple";
	const char *str4 = "banana";
	size_t n2 = 3;
	printf("\nTest 2:\n");
	int result2 = ft_strncmp(str3, str4, n2);
	printf("Result: %d\n", result2);

	// Test 3: Compare two strings with n = 0
	const char *str5 = "Hello";
	const char *str6 = "Hello";
	size_t n3 = 0;
	printf("\nTest 3:\n");
	int result3 = ft_strncmp(str5, str6, n3);
	printf("Result: %d\n", result3);

	return (0);
}
*/
