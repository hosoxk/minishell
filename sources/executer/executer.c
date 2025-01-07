/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 12:10:12 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 16:23:43 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_cmd(t_ast *ast_root, char ***env, pid_t *pids,
		bool is_first)
{
	char			*name;
	t_command_type	type;
	pid_t			pid;

	name = ast_root->args[0];
	type = CMD_NONE;
	if (is_first)
	{
		pid = fork();
		if (pid == -1)
			exit(1); /////////
		set_pid(pids, pid);
		if (!pid)
		{
			execute_custom_cmd_after_fork(name, ast_root->args, env, type);
			execute_build_in_cmd(name, ast_root->args, *env);
		}
	}
	else
	{
		execute_custom_cmd_after_fork(name, ast_root->args, env, type);
		execute_build_in_cmd(name, ast_root->args, *env);
	}
}

void	execute(t_ast *ast_root, char ***env, pid_t *pids, bool is_first)
{
	if (ast_root->type == WORD)
		execute_cmd(ast_root, env, pids, is_first);
	else if (ast_root->type == PIPE)
		do_pipe(ast_root, env, pids);
	else if (ast_root->type >= REDIRECT_IN && ast_root->type <= HEREDOC)
		do_redirection(ast_root, env, pids, is_first);
}

int	get_exit_code(pid_t *pids)
{
	int	status;
	int	i;
	int	len;

	len = get_len_pids(pids);
	i = 0;
	while (i < len)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid failed");
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = WTERMSIG(status) + 128;
		i++;
	}
	free(pids);
	return (status);
}

void	executor(t_ast *ast_root, char ***env)
{
	pid_t	*pids;

	if (execute_custom_cmd(ast_root, env))
		return ;
	pids = get_pid_list(ast_root);
	if (!pids)
		exit_clean("Failed to allocate pids", ast_root, *env);
	execute(ast_root, env, pids, true);
	g_exit_status = get_exit_code(pids);
}
