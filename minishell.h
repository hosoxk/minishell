/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 21:38:39 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/10/16 21:57:21 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
	// for chdir
# include "unistd.h"

enum	e_token
{
	EMPTY = 0,
	WORD = 1,
	WHITE_SPACE = 2,
	NEW_LINE = 3,
	QOUTE = 4,
	DOUBLE_QUOTE = 5,
	ESCAPE = 6,
	ENV = 7,
	PIPE = 8,
	REDIR_IN = 9,
	REDIR_OUT = 10,
	DOUBLEREDIR_IN = 11,
	DOUBLEREDIR_OUT = 12,
	HERE_DOC = 13,
};

typedef struct s_token
{
	char			*str;
	int				pos;
	int				type;
	struct s_token	*next;
}	t_token;
