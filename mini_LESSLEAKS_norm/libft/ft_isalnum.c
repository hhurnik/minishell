/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:28:52 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:24:20 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}
/*
int	main(void) {
	char test_chars[] = {'a', 'A', '5', '*', '?'};

	printf("Testing ft_isalnum function:\n");
	for (int i = 0; i < sizeof(test_chars) / sizeof(test_chars[0]); i++)
	{
		printf("Character '%c': %s\n", test_chars[i],
			ft_isalnum(test_chars[i]) ? "Alphanumeric" : "Not alphanumeric");
	}

	return (0);
}
*/
