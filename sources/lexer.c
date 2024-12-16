/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:43:38 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/16 17:17:40 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_special_case(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '$' || c == '\'' || c == '\"')
		return (true);
	return (false);
}

bool	lexer(char *line)
{
	while (*line)
	{
		if (is_white_space(*line))
			line++;
		else if (is_special_case(*line))
			//handle_special_case(*line); //TODO
			printf("%c is a special character\n", *line++);
		else
			line++;
	}
	return (true);
}
