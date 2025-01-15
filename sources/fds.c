/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 13:23:13 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/15 13:25:33 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_len(t_ast *tree)
{
	int	i;

	i = 0;
	if (!tree)
		return (0);
	if (tree->type == PIPE)
		i = 2;
	else if (tree->type == HEREDOC)
		i = 3;
	else if (tree->type >= REDIRECT_IN && tree->type <= APPEND)
		i = 3;
	i += get_len(tree->left);
	i += get_len(tree->right);
	return (i);
}

int	*get_fds(t_ast *tree)
{
	int	len;
	int	*fds;

	len = get_len(tree);
	if (len == 0)
		len = 1;
	fds = malloc(sizeof(int) * len);
	if (!fds)
		return (NULL);
	ft_memset(fds, -1, sizeof(int) * len);
	return (fds);
}

void	clear_fds(int *fds)
{
	int	*pt;

	if (!fds)
		return ;
	pt = fds;
	while (*pt != -1)
	{
		close(*pt);
		pt++;
	}
	free(fds);
}

void	add_fd(t_ast *tree, int fd)
{
	int	*fds;

	if (!tree->free_data->fds)
		return ;
	fds = tree->free_data->fds;
	while (*fds != -1)
		fds++;
	*fds = fd;
}

void	pop_fd(t_ast *tree)
{
	int	*fds;

	fds = tree->free_data->fds;
	if (!fds)
		return ;
	while (*fds != -1)
		fds++;
	fds--;
	close(*fds);
	*fds = -1;
}
