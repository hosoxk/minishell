/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:10:12 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/03 16:57:01 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd(t_ast *ast_root, char ***env)
{
	char			*name;
	t_command_type	type;

	name = ast_root->args[0];
	type = CMD_NONE;
	execute_custom_cmd_after_fork(name, ast_root->args, env, type);
	execute_build_in_cmd(name, ast_root->args, *env);
	exit(1);
	// cleanup
}

void	execute(t_ast *ast_root, char ***env, pid_t *pids)
{
	if (ast_root->type == WORD) //////////
		execute_cmd(ast_root, env);
	else if (ast_root->type == PIPE)
	{
		do_pipe(ast_root, env, pids);
	}
	else if (ast_root->type >= REDIRECT_IN && ast_root->type <= HEREDOC)
	{
		do_redirection(ast_root, env, pids);
	}
}

int	get_exit_code(pid_t *pids)
{
	int	status;
	int i;

	i = get_len_pids(pids);
	while (i > 0)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid failed");
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		i--;
	}
	free(pids);
	return (status);
}

void	executor(t_ast *ast_root, char ***env)
{
	pid_t	*pids;
	int		status;

	if (execute_custom_cmd(ast_root, env))
		return ;
	pids = get_pid_list(ast_root);
	pids[0] = fork();
	if (pids[0] == -1)
		exit(1);
	if (!pids[0])
		execute(ast_root, env, pids);
	status = get_exit_code(pids); ////////
}
