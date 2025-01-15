/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:25:49 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/08 12:30:29 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cleanup_heredoc(int *p_fd, char *line, int fd_in, int fd_out)
{
	if (line)
		free(line);
	close(p_fd[1]);
	dup2(fd_out, STDOUT_FILENO);
	dup2(fd_in, STDIN_FILENO);
}

static void	setup(int *p_fd, t_ast *ast_root, int *fd_in, int *fd_out)
{
	close(p_fd[0]);
	*fd_in = dup(STDIN_FILENO);
	*fd_out = dup(STDOUT_FILENO);
	dup2(ast_root->free_data->fd_out, STDOUT_FILENO);
	dup2(ast_root->free_data->fd_in, STDIN_FILENO);
}

static void	here_doc_put_in(t_ast *ast_root, int *p_fd)
{
	char	*ret;
	int		fd_in;
	int		fd_out;

	setup(p_fd, ast_root, &fd_in, &fd_out);
	while (1)
	{
		ret = readline("> ");
		if (!ret)
		{
			cleanup_heredoc(p_fd, ret, fd_in, fd_out);
			print_error("warning: here-document is delimited by \
				end-of-file! :o");
			exit(0);
		}
		if (ft_strcmp(ret, ast_root->file) == 0)
		{
			cleanup_heredoc(p_fd, ret, fd_in, fd_out);
			exit(0);
		}
		ft_putendl_fd(ret, p_fd[1]);
		free(ret);
	}
}

bool	init_heredoc(t_ast *ast_root)
{
	int		p_fd[2];
	pid_t	pid;
	int		temp_exit_status;

	if (pipe(p_fd) == -1)
		return (false);
	pid = fork();
	if (pid == -1)
		return (false);
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
	return (true);
}
