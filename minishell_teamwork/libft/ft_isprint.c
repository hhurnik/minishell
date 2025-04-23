/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:41 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:27:50 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c)
	{
		if ((c > 31 && c < 127))
			return (1);
	}
	return (0);
}
/*
int	main(void) {
	char test_chars[] = {31, 'A', 127, '*', '?'};

	printf("Testing ft_isprint function:\n");
	printf("Character '%c': %s\n", test_chars[0],
		ft_isprint(test_chars[0]) ? "Printable" : "Not printable");
	printf("Character '%c': %s\n", test_chars[1],
		ft_isprint(test_chars[1]) ? "Printable" : "Not printable");
	printf("Character '%c': %s\n", test_chars[2],
		ft_isprint(test_chars[2]) ? "Printable" : "Not printable");
	printf("Character '%c': %s\n", test_chars[3],
		ft_isprint(test_chars[3]) ? "Printable" : "Not printable");
	printf("Character '%c': %s\n", test_chars[4],
		ft_isprint(test_chars[4]) ? "Printable" : "Not printable");

	return (0);
}
*/
