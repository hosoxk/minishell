/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 10:22:45 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/06 09:50:49 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char *make_new_value(t_token *token, int index, char *env_value, int len)
{
	char *new_value;
	
	new_value = malloc(sizeof(char) * (ft_strlen(token->value) - len
				+ ft_strlen(env_value) + 1));
	if (!new_value)
		return (false); //// error gooiene
	ft_strlcpy(new_value, token->value, index + 1);
	ft_strcpy(new_value + index - 1, env_value);
	ft_strcpy(new_value + index - 1 + ft_strlen(env_value), &token->value[index
			+ len]);
	return (new_value);
}

static int	find_length(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_' || str[len] == '-' || str[len] == '?')) 
		len++;
	return (len);
}

static bool	insert_env(t_token *token, int index, char **env)
{
	int		len;
	char	*env_value;
	char	*new_value;
	bool	free_env_value;

	len = find_length(&token->value[index]);
	env_value = get_env_value(token, index, len, env);
	free_env_value = false;
	if (!env_value && token->value[index] == '?')
	{
		free_env_value = true;
		env_value = ft_itoa(g_exit_status);
	}
	else if (!env_value)
		env_value = "";
	new_value = make_new_value(token, index, env_value, len);
	if (!new_value)
		return (false);
	if (free_env_value)
		free(env_value);
	free(token->value);
	token->value = new_value;
	return (true);
}

static void	expend_token(t_token *token, char **env)
{
	char	*dollar;
	int		index;

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

void	kobe_expander(t_token *token_list, char **env)
{
	while (token_list)
	{
		if (token_list->type == VARIABLE || token_list->type == WORD
			|| token_list->type == DOUBLE_QUOTED_STRING)
			expend_token(token_list, env);
		token_list = token_list->next;
	}
}
