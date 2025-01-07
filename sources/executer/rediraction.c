/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediraction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:59:33 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 15:43:34 by kvanden-         ###   ########.fr       */
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
	{
		perror("Perror");
		ft_putendl_fd(file, STDERR_FILENO);
	}
	return (ret);
}

void	init_redirection(t_ast *ast_root)
{
	int	fd;

	fd = open_file(ast_root->file, ast_root->type);
	if (ast_root->type == REDIRECT_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
}

void	do_redirection(t_ast *ast_root, char ***env, pid_t *pids, bool is_first)
{
	if (ast_root->type == HEREDOC)
		init_heredoc(ast_root);
	else
		init_redirection(ast_root);
	execute(ast_root->left, env, pids, is_first);
}
