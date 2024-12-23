/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:36:00 by yde-rudd          #+#    #+#             */
/*   Updated: 2024/12/18 14:31:41 by yde-rudd         ###   ########.fr       */
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
# define BOLD_RED "\033[31m"
# define RESET "\033[0m"

# define ARG_SIZE 8;

typedef enum e_token_type
{
	WORD,
	PIPE, // ||
	REDIRECT_IN, // <
	REDIRECT_OUT, // >
	APPEND, // >>
	HEREDOC, // <<
	VARIABLE, // $VAR
	QUOTED_STRING, // '...'
	DOUBLE_QUOTED_STRING, // "..."
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next; // pointer to the next token
}	t_token;

typedef struct s_ast
{
	t_token_type	type;
	char		**args; // array of arguments for commands
	char		*file; // file name for redirection
	struct s_ast	*left; // left child (for pipes and redirects)
	struct s_ast	*right; // right child
}	t_ast;

// *** LEXER & HANDLER ***
void	lexer(char *line, t_token **token_list);
void	handle_var(char **line, t_token **token_list);
void	handle_redirect(char **line, t_token **token_list);
void	handle_quoted_str(char **line, t_token **token_list);
void	add_token_to_list(t_token **token_list, char *value, t_token_type type);
bool    validate_token_sequence(t_token *tokens);

// *** PARSER ***
t_ast   *parse_ast(t_token **tokens);
void    add_argument(char ***args, int *size, int *count, const char *value);
t_ast	*create_ast_node(t_token_type type);
t_ast	*create_pipe_node(t_ast *left_node, t_token **tokens);
t_ast   *create_redirection_node(t_ast *left_node, t_token **tokens);
t_ast   *create_command_node(t_ast *command_node, t_token **tokens, int *size, int *count);

// *** UTILS ***
bool	is_special_case(char c);
void	print_tokens(t_token **token_list);
void    print_ast(t_ast *node, int depth);

// *** CLEAN ***
void    free_token_list(t_token **token_list);
void    free_ast(t_ast *node);

#endif
