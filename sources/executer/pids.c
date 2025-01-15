/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pids.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:07:07 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/07 15:53:44 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_len_ast(t_ast *ast_root, int len)
{
	int	i;

	if (!ast_root)
		return (0);
	if (ast_root->type == WORD)
		return (1);
	i = 0;
	i += get_len_ast(ast_root->left, len);
	i += get_len_ast(ast_root->right, len);
	return (i);
}

pid_t	*get_pid_list(t_ast *ast_root)
{
	pid_t	*pids;

	pids = ft_calloc(sizeof(pid_t), get_len_ast(ast_root, 0) + 1);
	if (!pids)
		return (NULL);
	return (pids);
}

void	set_pid(pid_t *pids, pid_t pid)
{
	int	i;

	i = 0;
	while (pids[i])
		i++;
	pids[i] = pid;
}

int	get_len_pids(pid_t *pids)
{
	int	i;

	i = 0;
	while (pids[i])
		i++;
	return (i);
}
