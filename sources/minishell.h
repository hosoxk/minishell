/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:36:00 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/16 16:34:37 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

# define BOLD_MAGENTA "\033[35m"
# define RESET "\033[0m"

typedef enum e_token_type
{
	WORD,
	PIPE, // ||
	REDIRECT_IN, // <
	REDIRECT_OUT, // >
	APPEND, // >>
	HEREDOC, // <<
	VARIABLE, // $VAR
	QUOTED_STRING, // '...' or "..."
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
}	t_token;

// *** LEXER ***
bool	lexer(char *line);

#endif
