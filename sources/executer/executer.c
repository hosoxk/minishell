/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:10:12 by kvanden-          #+#    #+#             */
/*   Updated: 2024/12/30 15:32:03 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_pipe(t_ast *ast_root, char ***env)
{
	// TODO
	executor(ast_root->left, env);
	executor(ast_root->right, env);
}

static void	execute_cmd(t_ast *ast_root, char ***env)
{
	char			*name;
	t_command_type	type;

	name = ast_root->args[0];
	type = CMD_NONE;
	execute_custom_cmd_after_fork(name, ast_root->args, env, type);
	execute_build_in_cmd(name, ast_root->args, *env);
}

static void	execute(t_ast *ast_root, char ***env)
{
	if (ast_root->type == WORD)
		execute_cmd(ast_root, env);
	else if (ast_root->type == PIPE)
	{
		do_pipe(ast_root, env);
	}
}

void	executor(t_ast *ast_root, char ***env)
{
	pid_t	pid;
    int status;

	if (execute_custom_cmd(ast_root->args[0], ast_root->args, env, ast_root->type))
		return ;

	pid = fork();
	if (pid == -1)
		exit(1);
	if (!pid)
		execute(ast_root, env);
	else if (waitpid(pid, &status, 0) == -1)
		perror("waitpid failed");
}
