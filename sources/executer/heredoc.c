/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:25:49 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/07 13:58:27 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cleanup_heredoc(int *p_fd, char *line)
{
	if (line)
		free(line);
	close(p_fd[1]);
}

static void	here_doc_put_in(t_ast *ast_root, int *p_fd)
{
	char	*ret;

	close(p_fd[0]);
	while (1)
	{
		ret = readline("> ");
		if (!ret)
		{
			cleanup_heredoc(p_fd, ret);
			print_error("warning: here-document is delimited by \
				end-of-file! :o");
			exit(0);
		}
		if (ft_strcmp(ret, ast_root->file) == 0)
		{
			cleanup_heredoc(p_fd, ret);
			exit(0);
		}
		ft_putendl_fd(ret, p_fd[1]);
		free(ret);
	}
}

void	init_heredoc(t_ast *ast_root)
{
	int		p_fd[2];
	pid_t	pid;
	int		temp_exit_status;

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
		close(p_fd[0]);
		waitpid(pid, &temp_exit_status, 0);
		g_exit_status = temp_exit_status;
	}
}
