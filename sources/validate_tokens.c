#include "minishell.h"

static bool	check_pipe(t_token *token)
{
	if (!token->next || token->next->type == PIPE)
		return (printf(BOLD_RED"Syntax error: near unexpected token '|'\n"RESET), false);
	if (!token->next || token->next->type != WORD)
		return (printf(BOLD_RED"Syntax error: missing command after pipe\n"RESET), false);
	return (true);
}

static void	get_command_type(t_token *token)
{
	if (ft_strcmp(token->value, "echo") == 0)
		token->command_type = CMD_ECHO;
	else if (ft_strcmp(token->value, "cd") == 0)
		token->command_type = CMD_CD;
	else if (ft_strcmp(token->value, "pwd") == 0)
		token->command_type = CMD_PWD;
	else if (ft_strcmp(token->value, "export") == 0)
		token->command_type = CMD_EXPORT;
	else if (ft_strcmp(token->value, "unset") == 0)
		token->command_type = CMD_UNSET;
	else if (ft_strcmp(token->value, "env") == 0)
		token->command_type = CMD_ENV;
	else if (ft_strcmp(token->value, "exit") == 0)
		token->command_type = CMD_EXIT;
}

bool	is_valid_command(t_token *token)
{
	int		i;
	const char	*valid_commands[8];

	i = 0;
	valid_commands[0] = "echo";
	valid_commands[1] = "cd";
	valid_commands[2] = "pwd";
	valid_commands[3] = "export";
	valid_commands[4] = "unset";
	valid_commands[5] = "env";
	valid_commands[6] = "exit";
	valid_commands[7] = "NULL";
	while(valid_commands[i]) // i++
	{
		if (ft_strcmp(token->value, valid_commands[i]) == 0)
		{
			get_command_type(token);
			return (printf("\nCommand found!\n\n"), true);
		}
		i++;
	}
	return (false);
}

bool	validate_token_sequence(t_token *tokens)
{
	t_token	*prev_token;

	prev_token = NULL;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			check_pipe(tokens);
			if (!prev_token)
				return (printf(BOLD_RED"Syntax error: unexpected pipe at the beginning\n"RESET), false);
			if (!tokens->next || tokens->next->type == PIPE)
				return (printf(BOLD_RED"Syntax error: invalid pipe sequence\n"RESET), false);
			if (!tokens->next || tokens->next->type != WORD)
				return (printf(BOLD_RED"Syntax error: missing command after pipe\n"RESET), false);
		}
		if ((tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT ||
			tokens->type == APPEND || tokens->type == HEREDOC))
		{
			if (!tokens->next || tokens->next->type != WORD)
				return (printf(BOLD_RED"Syntax error: missing target for redirection\n"RESET), false);
		}
		if ((tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT || tokens->type == APPEND
			|| tokens->type == HEREDOC || tokens->type == PIPE) && !tokens->next)
			return (printf(BOLD_RED"Syntax error: invalid token at the beginning\n"RESET), false);
		if (tokens->type == WORD)
			is_valid_command(tokens);
		prev_token = tokens;
		tokens = tokens->next;
	}
	return (true);
}
