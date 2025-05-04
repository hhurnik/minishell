/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:59 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:33:13 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;

	p = s;
	while (n > 0)
	{
		*p = (unsigned char)c;
		p++;
		n--;
	}
	return (s);
}
/*
int	main(void) {
	char str[20];

	// Test 1: Set first 10 bytes of str to 'A'
	ft_memset(str, 'A', 10);
	printf("After memset: %s\n", str);

	// Test 2: Set next 5 bytes of str to 'B'
	ft_memset(str + 10, 'B', 5);
	printf("After memset: %s\n", str);

	return (0);
}
*/
