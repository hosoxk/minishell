/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_custom_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 13:00:31 by kvanden-          #+#    #+#             */
/*   Updated: 2024/12/30 18:01:28 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	execute_custom_cmd(char *name, char **argv, char ***env,
		t_command_type type)
{
	(void)type;

	if (ft_strcmp(name, "export") == 0)
		export(env, argv);
	else if (ft_strcmp(name, "unset") == 0)
		unset(env, argv);
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
		my_pwd(*env);
	else
		return ;
	// else if (ft_strcmp(token->value, "cd") == 0)
	// 	token->command_type = CMD_CD;
	// else if (ft_strcmp(token->value, "pwd") == 0)
	// 	token->command_type = CMD_PWD;
	// else if (ft_strcmp(token->value, "export") == 0)
	// 	token->command_type = CMD_EXPORT;
	// else if (ft_strcmp(token->value, "unset") == 0)
	// 	token->command_type = CMD_UNSET;
	// else if (ft_strcmp(token->value, "env") == 0)
	// 	token->command_type = CMD_ENV;
	// else if (ft_strcmp(token->value, "exit") == 0)
	// 	token->command_type = CMD_EXIT;
	exit(0);
}
