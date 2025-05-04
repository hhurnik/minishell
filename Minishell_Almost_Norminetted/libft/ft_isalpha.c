/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:23 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:25:03 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if (c)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			return (1);
	}
	return (0);
}

/*
int	main(void) {
	char test_chars[] = {'a', 'A', '5', '*', '?'};

	printf("Testing ft_isalpha function:\n");
	for (int i = 0; i < sizeof(test_chars) / sizeof(test_chars[0]); i++)
	{
		printf("Character '%c': %s\n", test_chars[i],
			ft_isalpha(test_chars[i]) ? "Alphabetic" : "Not alphabetic");
	}

	return (0);
}
*/
