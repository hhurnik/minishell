/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:01:13 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:37:06 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	dst_size;

	i = ft_strlen(dst);
	dst_size = ft_strlen(dst);
	j = 0;
	if (size && ft_strlen(dst) <= size)
	{
		while (src[j] != '\0' && i < (size - 1))
		{
			dst[i] = src[j];
			i++;
			j++;
		}
		dst[i] = '\0';
	}
	if (size == 0)
		dst_size = 0;
	if (size < dst_size)
		dst_size = size;
	return (dst_size + ft_strlen((char *)src));
}
/*
int	main(void) {
	char dst[20] = "Hello";
	const char *src = ", world!";
	size_t size = sizeof(dst);

	// Test 1: Concatenate src to dst with size = 20
	printf("Test 1:\n");
	size_t result1 = ft_strlcat(dst, src, size);
	printf("Result string: %s\n", dst);
	printf("Result length: %zu\n", result1);

	// Test 2: Concatenate src to dst with size = 10
	char dst2[10] = "Hello";
	printf("\nTest 2:\n");
	size_t result2 = ft_strlcat(dst2, src, sizeof(dst2));
	printf("Result string: %s\n", dst2);
	printf("Result length: %zu\n", result2);

	return (0);
}
*/
