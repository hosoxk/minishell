/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:15:46 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/17 15:15:47 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token **token_list)
{
	t_token *current = *token_list;
	while (current)
	{
		printf("Token: %s, Type: %d\n", current->value, current->type);
		current = current->next;
	}
}

bool	is_special_case(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '$' || c == '\'' || c == '\"')
		return (true);
	return (false);
}
