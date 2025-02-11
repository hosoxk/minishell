/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:39:13 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/02/10 09:07:04 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*init_token(char *value, t_token_type type, char **env)
{
	t_token	*new_token;

	new_token = malloc(sizeof (t_token));
	if (!new_token)
		return (print_error_status("Failure mallocing space for new_token",
				env), NULL);
	new_token->type = type;
	new_token->next = NULL;
	new_token->value = NULL;
	new_token->value = ft_strdup(value);
	if (!new_token->value && value)
		return (free(new_token), print_error("Error: "),
			print_error_status("failure ft_strdup in add_token",
				env), NULL);
	return (new_token);
}

/*	Allocate space for a new t_token, if token_list is present,
 *	token iterates to the end of the list
 */

bool	add_token(t_token **token_list, char *value,
	t_token_type type, char **env)
{
	t_token	*new_token;
	t_token	*current;

	if (!token_list)
		return (print_error("null parameter found for add_token"),
			false);
	current = NULL;
	new_token = init_token(value, type, env);
	if (!new_token)
		return (false);
	if (!*token_list)
		*token_list = new_token;
	else
	{
		current = *token_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (true);
}
