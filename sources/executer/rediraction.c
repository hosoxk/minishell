/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediraction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:59:33 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/08 12:31:53 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_file(char *file, t_token_type mode)
{
	int	ret;

	if (mode == REDIRECT_IN)
		ret = open(file, O_RDONLY, 0666);
	if (mode == REDIRECT_OUT)
		ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (mode == APPEND)
		ret = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (ret == -1)
		perror(file);
	return (ret);
}

static bool	init_redirection(t_ast *ast_root)
{
	int	fd;

	fd = open_file(ast_root->file, ast_root->type);
	if (fd == -1)
		return (false);
	if (ast_root->type == REDIRECT_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (true);
}

bool	do_redirection(t_ast *ast_root, char ***env, pid_t *pids, bool is_first)
{
	int		fd_in;
	int		fd_out;
	bool	can_execute;

	fd_in = dup(STDIN_FILENO);
	fd_out = dup(STDOUT_FILENO);
	if (ast_root->type == HEREDOC)
		can_execute = init_heredoc(ast_root);
	else
		can_execute = init_redirection(ast_root);
	if (can_execute)
		can_execute = execute(ast_root->left, env, pids, is_first);
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_in);
	close(fd_out);
	return (can_execute);
}
