/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:52:33 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/07 14:06:03 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_name(char **argv, char **env)
{
	char	*name;
	char	*is_location;

	is_location = ft_strchr(argv[1], '=');
	if (!is_location)
		return (NULL);
	name = ft_substr(argv[1], 0, is_location - argv[1]);
	if (!name)
		print_error_status("malloc failed", env);
	return (name);
}

static char	*get_value(char **argv, char *name, char **env)
{
	char	*value;

	if (argv[1][ft_strlen(argv[1]) - 1] != '=')
		value = argv[1] + ft_strlen(name) + 1;
	else if (!argv[2])
		value = "";
	else
		value = argv[2];
	if (!value)
		print_error_status("malloc failed", env);
	return (value);
}

static void	print_export(char **env)
{
	while (*env)
	{
		if (ft_strcmp(*env, "NULL") != 0)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putendl_fd(*env, 1);
		}
		env++;
	}
}

bool	my_export(char ***env, char **argv)
{
	char	*name;
	char	*value;
	bool	return_val;

	if (!argv[1])
		return (print_export(*env), true);
	name = get_name(argv, *env);
	if (!name)
		return (true);
	value = get_value(argv, name, *env);
	if (!value)
	{
		free(name);
		return (false);
	}
	return_val = update_env(name, value, env);
	free(name);
	return (return_val);
}
