/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:30:13 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/27 19:39:51 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	result;

	result = 0;
	while (*s)
	{
		if (*s != c)
		{
			result++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (result);
}

char	**ft_split(const char *s, char c)
{
	char	**result;
	size_t	i;
	size_t	size;

	if (!s)
		return (0);
	i = 0;
	result = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!result)
		return (0);
	while (*s)
	{
		if (*s != c)
		{
			size = 0;
			while (*s && *s != c && ++size)
				s++;
			result[i++] = ft_substr(s - size, 0, size);
		}
		else
			s++;
	}
	result[i] = 0;
	return (result);
}
/*
int	main(void) {
	// Test 1: Split a string by space
	const char *str1 = "Hello world! This is a test.";
	char **result1 = ft_split(str1, ' ');
	printf("Test 1:\n");
	for (int i = 0; result1[i] != NULL; ++i) {
		printf("%s\n", result1[i]);
	}
	ft_free_strarray(result1);

	// Test 2: Split a string by comma
	const char *str2 = "apple,banana,orange";
	char **result2 = ft_split(str2, ',');
	printf("\nTest 2:\n");
	for (int i = 0; result2[i] != NULL; ++i) {
		printf("%s\n", result2[i]);
	}
	ft_free_strarray(result2);

	// Test 3: Split a string with multiple consecutive delimiters
	const char *str3 = "One  Two   Three";
	char **result3 = ft_split(str3, ' ');
	printf("\nTest 3:\n");
	for (int i = 0; result3[i] != NULL; ++i) {
		printf("%s\n", result3[i]);
	}
	ft_free_strarray(result3);

	return (0);
}

// Free function for the string array returned by ft_split
void	ft_free_strarray(char **str_array) {
	if (str_array == NULL) return ;
	for (int i = 0; str_array[i] != NULL; ++i) {
		free(str_array[i]);
	}
	free(str_array);
}
*/
