/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:28 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:26:51 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}
/*
int	main(void)
{
	char	test_chars[] = {0, 'A', 128, '?'};

	printf("Testing ft_isascii function:\n");
	printf("Character '%c': %s\n", test_chars[0],
		ft_isascii(test_chars[0]) ? "ASCII" : "Not ASCII");
	printf("Character '%c': %s\n", test_chars[1],
		ft_isascii(test_chars[1]) ? "ASCII" : "Not ASCII");
	printf("Character '%c': %s\n", test_chars[2],
		ft_isascii(test_chars[2]) ? "ASCII" : "Not ASCII");
	printf("Character '%c': %s\n", test_chars[3],
		ft_isascii(test_chars[3]) ? "ASCII" : "Not ASCII");
	return (0);
}
*/
