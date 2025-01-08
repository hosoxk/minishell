/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:56:42 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/08 15:05:37 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_clean(t_ast *node, char **env, int exit_status)
{
	if (node)
		free_ast(node->root);
	if (env)
		ft_free_tab(env);
	if (exit_status != -1)
		exit(exit_status);
	if (errno == ENOENT || errno == EBADF)
		exit(127);
	if (errno == EACCES)
		exit(126);
	exit(errno);
}
