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

#include "../minishell.h"

static char	*get_new_env(int i, int size, char **env)
{
	char	*temp;

	if (i < size)
		temp = ft_strdup(env[i]);
	else
		temp = ft_strdup("NULL");
	return (temp);
}

static char	**expent_env(char **env)
{
	int		size;
	int		i;
	char	**new_env;
	char	*temp;

	size = ft_tab_len(env);
	new_env = ft_calloc(size * 2, sizeof(char *));
	if (!new_env)
		return (print_error("malloc failed"), NULL);
	i = 0;
	while (i < ((size * 2) - 1))
	{
		temp = get_new_env(i, size, env);
		if (!temp)
		{
			new_env[i] = NULL;
			ft_free_tab(new_env);
			return (print_error("malloc failed"), NULL);
		}
		new_env[i] = temp;
		i++;
	}
	ft_free_tab(env);
	return (new_env);
}

static int	get_location(char ***env, char **argv)
{
	char	*name;
	char	*is_location;
	int		index;
	char	**new_env;

	is_location = ft_strchr(argv[1], '=');
	if (!is_location)
		return (-1);
	name = ft_substr(argv[1], 0, is_location - argv[1]);
	if (!name)
		return (print_error("malloc failed"), -1);
	index = getenv_index(name, *env);
	free(name);
	if (index >= 0)
		return (index);
	index = getenv_index("NULL", *env);
	if (index >= 0)
		return (index);
	index = ft_tab_len(*env);
	new_env = expent_env(*env);
	if (!new_env)
		return (-1);
	*env = new_env;
	return (index);
}

static void	my_set_var(char ***env, char **argv, int index)
{
	char	*temp;

	if (argv[1][ft_strlen(argv[1]) - 1] != '=')
		temp = ft_strdup(argv[1]);
	else
		temp = ft_strjoin(argv[1], argv[2]);
	if (!temp)
		return (print_error("malloc failed"));
	free((*env)[index]);
	(*env)[index] = temp;
}

void	export(char ***env, char **argv)
{
	int	index;

	index = get_location(env, argv);
	if (index >= 0)
		my_set_var(env, argv, index);
}
