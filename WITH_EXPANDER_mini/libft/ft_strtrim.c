/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:03:40 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/25 21:39:34 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_in_set(const char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	char	*new_str;

	if (!s1 || !set)
		return (0);
	while (s1 && is_in_set(*s1, set))
	{
		if (is_in_set(((char)*s1), set) == 1)
			s1++;
		else
			break ;
	}
	len = ft_strlen(s1);
	while (len != 0)
	{
		if (is_in_set(s1[len - 1], set) == 1)
			len--;
		else
			break ;
	}
	new_str = malloc(len * sizeof(char) + 1);
	if (!new_str)
		return (0);
	ft_strlcpy(new_str, (char *)s1, len + 1);
	return (new_str);
}
/*
1: Helper function that checks if a character c is present
	in the set of characters set. It iterates through each character
	in the set using a while loop. If it finds a match, it returns 1,
	indicating that the character is in the set. If it doesn't find a
	match after checking all characters in the set, it returns 0.

2: Variables i and len, where i is used as a counter and len holds the
	length of the input string s1.

3: Allocates memory for the new trimmed string new_str using malloc,
	ensuring to allocate enough space for the trimmed string plus the
	null terminator.

4: Checks if 's1', 'set', or 'new_str' are NULL. If any of them are,
	it returns NULL to indicate an error or invalid input.

5: Skips over leading charactersa the beginning and at the end in 's1'
that are in the'set' using a while loop and the 'is_in_set' function.

6: Copy the 'len'(remaining after trimming) characters from 's1' to 'new_str'.

7: Add the null terminator at the end of new_str to ensure it is a valid C
string, and returns new_str.
*/
/*

int	main(void)
{
	const char	*original_string = "   Hello, World!   ";
	const char	*set = " ";
	char		*trimmed_string;

	trimmed_string = ft_strtrim(original_string, set);
	if (trimmed_string)
	{
		printf("Original string: \"%s\"\n", original_string);
		printf("Trimmed string: \"%s\"\n", trimmed_string);
		free(trimmed_string); // Remember to free allocated memory
	}
	else
	{
		printf("Error: Memory allocation failed.\n");
	}
	free()
	return (0);
}
*/
