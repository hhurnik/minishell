/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:01:17 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:37:32 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	i;

	i = 0;
	if (!src || !dst)
		return (0);
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] != '\0' && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
/*
int	main(void) {
	char dst[20];
	const char *src = "This is a test string.";

	// Test 1: Copy src to dst with size = 20
	printf("Test 1:\n");
	size_t result1 = ft_strlcpy(dst, src, sizeof(dst));
	printf("Result string: %s\n", dst);
	printf("Result length: %zu\n", result1);

	// Test 2: Copy src to dst with size = 10
	char dst2[10];
	printf("\nTest 2:\n");
	size_t result2 = ft_strlcpy(dst2, src, sizeof(dst2));
	printf("Result string: %s\n", dst2);
	printf("Result length: %zu\n", result2);

	return (0);
}
*/
