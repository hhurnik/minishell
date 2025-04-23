/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:48 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:30:38 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *dest, const void *src, size_t n)
{
	char	*str1;
	char	*str2;
	size_t	i;

	i = 0;
	str1 = (char *)dest;
	str2 = (char *)src;
	if (dest == NULL && src == NULL)
		return (0);
	if (i > n)
		return (0);
	while (i < n)
	{
		if (str1[i] != str2[i])
			return ((unsigned char)str1[i] - (unsigned char)str2[i]);
		i++;
	}
	return (0);
}
/*
Variable Declarations :

char *str1; and char *str2;: Pointers to characters used for comparing the
contents of two memory areas. size_t i;: A variable to iterate through bytes
in the loop.

Pointer Assignment:

str1 = (char *)dest;
and	str2 = (char *)src;: Casting generic pointers to char pointers allows
treating the data as bytes (1 byte per character).

Initial Condition Checks:

if (dest == NULL && src == NULL) return (0);: If both pointers are NULL,
	it assumes that the memory areas are identical.
if (i > n) return (0);: Checks if 'i' (loop counter) is greater than 'n'
(the number of bytes to compare). If true, it returns 0.

Byte Comparison in a Loop:

while (i < n) { ... }: The loop iterates for each byte to compare
(up to the specified n).
if (str1[i] != str2[i]) return ((unsigned char)str1[i]
	- (unsigned char)str2[i]);: Compares corresponding bytes. If they differ,
	it returns the difference between them (casted to unsigned char).

Returning the Result:

If the loop iterates through all bytes, and no differences are found,
	the function returns 0, indicating that the memory areas are identical.
In summary, this function compares n bytes between two memory areas and
returns the difference between them (or 0 if they are identical).
*/

/*
int	main(void) {
	char str1[] = "Hello, world!";
	char str2[] = "Hello, world!";
	char str3[] = "HELLO, WORLD!";
	char str4[] = "Hello, wonderful world!";

	printf("Comparing str1 and str2: %d\n", ft_memcmp(str1, str2, strlen(str1)
			+ 1));
	printf("Comparing str1 and str3: %d\n", ft_memcmp(str1, str3, strlen(str1)
			+ 1));
	printf("Comparing str1 and str4: %d\n", ft_memcmp(str1, str4, strlen(str1)
			+ 1));

	return (0);
}
*/
