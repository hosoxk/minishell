/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_custom_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:00:31 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 09:59:54 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	execute_custom_cmd(t_ast *ast_root, char ***env)
{
	char	*name;
	
	if (ast_root->type != WORD)
		return (false);
	name = ast_root->args[0];
	if (ft_strcmp(name, "export") == 0)
		export(env, ast_root->args);
	else if (ft_strcmp(name, "unset") == 0)
		unset(env, ast_root->args);
	else if (ft_strcmp(name, "cd") == 0)
		my_cd(*env, ast_root->args);
	else
		return (false);
	return (true);
}

void	execute_custom_cmd_after_fork(char *name, char **argv, char ***env,
		t_command_type type)
{
	(void)type;
	if (ft_strcmp(name, "echo") == 0)
		my_echo(argv);
	else if (ft_strcmp(name, "env") == 0)
		my_env(*env);
	else if (ft_strcmp(name, "pwd") == 0)
		my_pwd();
	else
		return ;
	exit(0);
}
