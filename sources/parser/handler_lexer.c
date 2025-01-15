#include "../minishell.h"

bool	handle_parentheses(char **line, t_token **token_list)
{
	bool	return_val;

	if (**line == '(')
		return_val = add_token_to_list(token_list, NULL, PARENTHESES_OPEN);
	else
		return_val = add_token_to_list(token_list, NULL, PARENTHESES_CLOSE);
	if (return_val)
		(*line)++;
	return (return_val);
}

bool	handle_pipe(char **line, t_token **token_list)
{
	(*line)++;
	if (**line == '|')
	{
		(*line)++;
		return (add_token_to_list(token_list, NULL, OR));
	}
	else
		return (add_token_to_list(token_list, "|", PIPE));
}

bool	handle_ampersand(char **line, t_token **token_list)
{
	(*line)++;
	if (**line == '&')
	{
		(*line)++;
		return (add_token_to_list(token_list, NULL, AND));
	}
	else
		return (false);
}
