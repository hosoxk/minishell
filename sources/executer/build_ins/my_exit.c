/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:30:52 by kvanden-          #+#    #+#             */
/*   Updated: 2025/02/06 15:58:43 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	_my_exit(char **argv)
{
	int	i;

	if (!argv[1])
	{
		g_exit_status = 0;
		return (true);
	}
	i = 0;
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
			return (print_error_exit_status("exit: \
                one numeric argument required", 2), true);
		i++;
	}
	if (argv[2])
		return (print_error_exit_status("exit: too many arguments", 1), false);
	g_exit_status = ft_atoi(argv[1]);
	return (true);
}

void	my_exit(char **argv)
{
	_my_exit(argv);
}

bool	my_exit_normal(char ***env, t_ast *node)
{
	if (node != node->free_data->root)
		return (false);
	printf("exit\n");
	if (_my_exit(node->args))
		exit_clean(node, *env, g_exit_status);
	return (true);
}
