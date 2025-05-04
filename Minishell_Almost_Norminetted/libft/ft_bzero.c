/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:11 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:22:46 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*dest;

	i = 0;
	dest = s;
	while (i < n)
		dest[i++] = 0;
}
/*
int	main(void)
{
	char	str[10] = "Hello";
	size_t	size;

	size = strlen(str);
	printf("Before ft_bzero: %s\n", str);
	ft_bzero(str, size);
	printf("After ft_bzero: ");
	for (size_t i = 0; i < size; i++) {
		printf("%d ", str[i]);
	}
	printf("\n");
	return (0);
}
*/
