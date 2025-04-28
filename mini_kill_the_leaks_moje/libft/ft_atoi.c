/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:31:13 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 20:14:32 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	n;
	int	result;

	result = 0;
	n = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-')
	{
		n = -1;
	}
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= 48 && *str <= 57)
		result = (result * 10) + (*str++ - 48);
	return (n * result);
}

/*
int	main(void)
{
	int		value;
	char	str[30];

	// assigning value to variable str
	strcpy(str, "   --+---+3256ab42");
	value = ft_atoi(str);
	printf("String value = %s, Integer value = %d", str, value);
	return (0);
}
*/
