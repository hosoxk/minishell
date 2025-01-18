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
	if (ft_strchr(token->value, '*'))
		if (!expand_wildcard(token))
			return (false);
	return (expand_var(token, env));
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
