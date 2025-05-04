/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:37 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:27:28 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if ((c >= 48 && c <= 57))
		return (1);
	return (0);
}
/*
int	main(void) {
	char test_chars[] = {'0', '9', 'A', '*', '?'};

	printf("Testing ft_isdigit function:\n");
	printf("Character '%c': %s\n", test_chars[0],
		ft_isdigit(test_chars[0]) ? "Digit" : "Not a digit");
	printf("Character '%c': %s\n", test_chars[1],
		ft_isdigit(test_chars[1]) ? "Digit" : "Not a digit");
	printf("Character '%c': %s\n", test_chars[2],
		ft_isdigit(test_chars[2]) ? "Digit" : "Not a digit");
	printf("Character '%c': %s\n", test_chars[3],
		ft_isdigit(test_chars[3]) ? "Digit" : "Not a digit");
	printf("Character '%c': %s\n", test_chars[4],
		ft_isdigit(test_chars[4]) ? "Digit" : "Not a digit");

	return (0);
}
*/
