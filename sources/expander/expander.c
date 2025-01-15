/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 10:22:45 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/08 15:58:04 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Allocates a new string and copies the value of a token into it with the
 *      substring from index to index + len replaced with the value of env_value.
 *
 * @param token The token whose value is being copied.
 * @param index The index at which the substring should be replaced.
 * @param env_value The value to replace the substring with.
 * @param len The length of the substring to be replaced.
 * @return The new string with the replaced substring.
 */
static char	*make_new_value(t_token *token, int index, char *env_value, int len)
{
	char	*new_value;

	new_value = malloc(sizeof(char) * (ft_strlen(token->value) - len
				+ ft_strlen(env_value) + 1));
	if (!new_value)
		return (print_error("malloc failed"), NULL);
	ft_strlcpy(new_value, token->value, index + 1);
	ft_strcpy(new_value + index - 1, env_value);
	ft_strcpy(new_value + index - 1 + ft_strlen(env_value), &token->value[index
		+ len]);
	return (new_value);
}

/**
 * @brief Returns the length of a substring in a string that only contains
 *        alphanumeric characters, '_', '-', and '?'.
 *
 * @param str The string in which to find the substring.
 * @return The length of the substring.
 */
static int	find_length(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'
			|| str[len] == '-' || str[len] == '?'))
		len++;
	return (len);
}

/**
 * @brief Inserts the value of an environment variable into a token.
 *
 * @param token The token into which to insert the environment variable.
 * @param index The index of the first character of the environment variable
 *              name in the token's value.
 * @param env The environment array.
 * @return true if the operation was successful, false otherwise.
 *
 * If the environment variable is not found, the token's value is not modified.
 * If the environment variable is '?', the exit status is inserted instead.
 */
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
		if (!env_value)
			return (false);
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

/**
 * @brief Expands a token by replacing '*' with the result of wildcard expansion
 *        and replacing '$' with the value of the environment variable that
 *        follows.
 *
 * @param token The token to expand.
 * @param env The environment array.
 * @return true if the operation was successful, false otherwise.
 *
 * This function expands a token by replacing '*' with the result of wildcard
 * expansion and replacing '$' with the value of the environment variable that
 * follows. If the environment variable is not found, the token's value is not
 * modified. If the environment variable is '?', the exit status is inserted
 * instead. If the token contains no '*' or '$', the function returns true
 * without modifying the token.
 */
static bool	expend_token(t_token *token, char **env)
{
	char	*dollar;
	int		index;

	while (true)
	{
		if (ft_strchr(token->value, '*'))
			if (!expand_wildcard(token))
				return (false);
		dollar = ft_strchr(token->value, '$');
		if (!dollar)
			return (true);
		index = dollar - token->value + 1;
		if (!insert_env(token, index, env))
			return (false);
	}
}

/**
 * @brief Expands all tokens in a linked list of tokens, replacing '*' with the
 *        result of wildcard expansion and replacing '$' with the value of the
 *        environment variable that follows.
 *
 * @param token_list The linked list of tokens to expand.
 * @param env The environment array.
 * @return true if the operation was successful, false otherwise.
 *
 * This function goes through a linked list of tokens and expands each token that
 * contains '*' or '$' characters. If the token contains '*', the function
 * replaces it with the result of wildcard expansion. If the token contains '$',
 * the function replaces it with the value of the environment variable that
 * follows. If the environment variable is not found, the token's value is not
 * modified. If the token contains no '*' or '$', the function returns true
 * without modifying the token. The function also changes the type of the
 * tokens to WORD after expansion is done.
 */
bool	expander(t_token *token_list, char **env)
{
	while (token_list)
	{
		if (token_list->type == VARIABLE || token_list->type == WORD
			|| token_list->type == DOUBLE_QUOTED_STRING)
		{
			if (!expend_token(token_list, env))
				return (false);
			token_list->type = WORD;
		}
		else if (token_list->type == QUOTED_STRING)
			token_list->type = WORD;
		token_list = token_list->next;
	}
	return (true);
}
