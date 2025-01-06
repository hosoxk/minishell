/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:25:49 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 10:21:05 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	here_doc_put_in(t_ast *ast_root, int *p_fd)
{
	char	*ret;

	close(p_fd[0]);
	while (1)
	{
        ret = readline("> ");
        if (!ret)
            return ; /////////////
        if (ft_strcmp(ret, ast_root->file) == 0)
		{
			free(ret);
			exit(0);
		}
		ft_putstr_fd(ret, p_fd[1]);
		ft_putstr_fd("\n", p_fd[1]);
		free(ret);
	}
}

void init_heredoc(t_ast *ast_root)
{
	int		p_fd[2];
	pid_t	pid;

	if (pipe(p_fd) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (!pid)
		here_doc_put_in(ast_root, p_fd);
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		wait(NULL);
	}
}
