/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 14:52:17 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:29:10 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_num(long num)
{
	int	count;

	count = 0;
	if (num < 0)
	{
		count++;
		num *= -1;
	}
	else if (num == 0)
		return (1);
	while ((unsigned int)num > 0)
	{
		count++;
		num = (unsigned int)num / 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*result;
	long	num_count;

	num_count = count_num(n);
	result = malloc((num_count + 1) * sizeof(char));
	if (n == 0)
		result[0] = 48;
	if (result == NULL)
		return (NULL);
	if (n < 0)
	{
		result[0] = '-';
		n *= -1;
	}
	result[num_count] = '\0';
	while ((num_count - 1) >= 0 && (unsigned int)n > 0)
	{
		result[num_count - 1] = ((unsigned int)n % 10) + 48;
		n = (unsigned int)n / 10;
		num_count--;
	}
	return (result);
}

/*
int	main(void) {
	int num = -12345;
	char *str = ft_itoa(num);
	if (str != NULL) {
		printf("Integer: %d\nString: %s\n", num, str);
		free(str); // Don't forget to free allocated memory
	} else {
		printf("Memory allocation failed\n");
	}
	return (0);
}
*/
