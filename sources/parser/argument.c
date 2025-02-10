/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 13:35:03 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/02/07 13:10:41 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	allocate_args(char ***args, int *size)
{
	*size = ARG_SIZE;
	*args = malloc(sizeof (char *) * (*size));
	if (!*args)
		print_error("Error: failure allocating arguments array");
	return (*args != NULL);
}

static bool	reallocate_args(char ***args, int *size)
{
	int	old_size;

	old_size = *size * sizeof (char *);
	*size += ARG_SIZE;
	*args = ft_realloc(*args, old_size, *size * sizeof (char *));
	if (!*args)
		print_error("Error: failure reallocating arguments array");
	return (*args != NULL);
}

bool	add_argument(char ***args, int *size, int *count, const char *value)
{
	if (!value)
	{
		print_error("Error: attempting to add NULL argument");
		return (false);
	}
	if (!*args)
	{
		if (!allocate_args(args, size))
			return (false);
	}
	else if (*count >= (*size - 1))
		if (!reallocate_args(args, size))
			return (false);
	(*args)[*count] = ft_strdup(value);
	if (!(*args)[*count])
	{
		print_error("Error: ft_strdup failed in add_argument\n");
		return (false);
	}
	(*count)++;
	(*args)[*count] = NULL;
	return (true);
}
