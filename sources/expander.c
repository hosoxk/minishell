/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 10:22:45 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/03 11:36:34 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_value(t_token *token, int index, int len, char **env)
{
	char	*env_name;
	char	*env_value;

	env_name = ft_strndup(&token->value[index], len);
	if (!env_name)
		return (NULL);
	env_value = getenv_stript(env_name, env);
	free(env_name);
	return (env_value);
}

static int	find_length(char *str)
{
	int	len;

	len = 0;
	while (str[len] && ft_isalnum(str[len]))
		len++;
	return (len);
}

static bool	insert_env(t_token *token, int index, char **env)
{
	int		len;
	char	*env_value;
	char	*new_value;

	len = find_length(&token->value[index]);
	env_value = get_env_value(token, index, len, env);
	if (!env_value)
		env_value = "";
	new_value = malloc(sizeof(char) * (ft_strlen(token->value) - len
				+ ft_strlen(env_value) + 1));
	if (!new_value)
		return (false); //// error gooiene
	ft_strlcpy(new_value, token->value, index + 1);
	ft_strcpy(new_value + index - 1, env_value);
	ft_strcpy(new_value + index - 1 + ft_strlen(env_value), &token->value[index
			+ len]);
	free(token->value);
	token->value = new_value;
    return (true);
}

static void	expend_token(t_token *token, char **env)
{
	char	*dollar;
	int		index;

	// if (token->type == VARIABLE) /// laat de dollar teken staan
	// 	index = 0;
	while (true)
    {
		dollar = ft_strchr(token->value, '$');
		if (!dollar)
			return ;
		index = dollar - token->value + 1;
        if (!insert_env(token, index, env))
            return ;
	}
}

void	expander(t_token *token_list, char **env)
{
	while (token_list)
	{
		if (token_list->type == VARIABLE || token_list->type == WORD
			|| token_list->type == DOUBLE_QUOTED_STRING)
			expend_token(token_list, env);
		token_list = token_list->next;
	}
}