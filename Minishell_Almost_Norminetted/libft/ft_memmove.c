/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:55 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:31:51 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*d;
	char	*s;

	d = (char *)dest;
	s = (char *)src;
	if (dest <= src)
	{
		while (n--)
		{
			*d++ = *s++;
		}
	}
	else if (dest > src)
	{
		d += n - 1;
		s += n - 1;
		while (n--)
			*d-- = *s--;
	}
	return (dest);
	if (!dest && !src)
		return (dest);
}
/*

int	main(void) {
	char str[] = "This is a test string.";
	char buffer[50];

	// Move part of the string to the beginning
	ft_memmove(buffer, str + 5, 10);

	printf("Original string: %s\n", str);
	printf("After memmove: %s\n", buffer);

	return (0);
}
*/
