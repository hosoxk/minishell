/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:30:52 by kvanden-          #+#    #+#             */
/*   Updated: 2025/02/07 13:14:38 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	_my_exit(char **argv, char **env)
{
	int	i;

	if (!argv[1])
	{
		set_exit_status(0, env);
		return (true);
	}
	i = 0;
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
			return (print_error_exit_status("exit: \
                one numeric argument required", 2, env), true);
		i++;
	}
	if (argv[2])
		return (print_error_exit_status("exit: too many arguments", 1, env), false);
	set_exit_status(atoi(argv[1]), env);
	return (true);
}

void	my_exit(char **argv, char **env)
{
	_my_exit(argv, env);
}

bool	my_exit_normal(char ***env, t_ast *node)
{
	if (node != node->free_data->root)
		return (false);
	printf("exit\n");
	if (_my_exit(node->args, *env))
		exit_clean(node, *env, get_exit_status(*env));
	return (true);
}
