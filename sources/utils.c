#include "minishell.h"

bool	is_special_case(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '$' || c == '\'' || c == '\"')
		return (true);
	return (false);
}
