/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:00:17 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/03/29 16:37:53 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*tmp;

	if (nmemb == 0 || size == 0)
	{
		nmemb = 1;
		size = 1;
	}
	if (2147483647 / nmemb < size)
		return (NULL);
	tmp = malloc(nmemb * size);
	if (!tmp)
		return (NULL);
	ft_bzero(tmp, nmemb * size);
	return (tmp);
}
/*
Calloc function dynamicaly allocates bites in memory and
initializes allocated memory with zero.
Void* in this function means it can be used to dynamically
allocate different data types.

1: allocate nmemb * size bytes in memory with malloc.
The pointer to the allocated memory is assigned to "tmp" variable.

2:check, if the memory allocation was succesful. If the "tmp"
pointer is NULL, function returns NULL, sigbaling mem allocation issue.

3:A loop that iterates through each byte in the allocated memory
and sets it to 0. This ensures the entire allocated memory is
initialized with zeros.

4: Returning the pointer to the dynamically allocated and initialized
memory.This memory can be used to store data, with its content set to 0.

*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(void)
{
	size_t	nmemb1;
	size_t	size1;
	int		*arr1;
	size_t	nmemb2;
	size_t	size2;
	int		*arr2;

	// Test case 1
	nmemb1 = 5;
	size1 = 4;
	arr1 = (int *)ft_calloc(nmemb1, size1);
	if (arr1 != NULL)
	{
		printf("Test Case 1: Memory allocated successfully.\n");
		// Print and verify the allocated memory
		for (size_t i = 0; i < nmemb1; ++i)
		{
			printf("%d ", arr1[i]);
		}
		printf("\n");
		free(arr1);
	}
	else
	{
		printf("Test Case 1: Memory allocation failed.\n");
	}
	// Test case 2
	nmemb2 = 0;
	size2 = 4;
	arr2 = (int *)ft_calloc(nmemb2, size2);
	if (arr2 != NULL)
	{
		printf("Test Case 2: Memory allocated successfully.\n");
		// Print and verify the allocated memory
		for (size_t i = 0; i < nmemb2; ++i)
		{
			printf("%d ", arr2[i]);
		}
		printf("\n");
		free(arr2);
	}
	else
	{
		printf("Test Case 2: Memory allocation failed.\n");
	}
	return (0);
}
*/
