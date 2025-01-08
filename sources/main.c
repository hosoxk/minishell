/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/08 17:20:02 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int	g_exit_status = 0;

static bool	check_input(int argc, char **envp)
{
	if (argc != 1)
		return (print_error("Correct usage: ./minishell"), false);
	if (!envp)
		return (print_error("Failure locating envp"), false);
	return (true);
}

void	move_cmds(t_ast **node)
{
	t_ast	*cmd;
	t_ast	*first_red;
	t_ast	*cur_red;
	t_ast	*next_red;

	if (!(*node))
		return ;
	cmd = *node;
	first_red = cmd->left;
	if (cmd->type == WORD && first_red && (first_red->type >= REDIRECT_IN
			&& first_red->type <= HEREDOC))
	{
		next_red = first_red->right;
		cur_red = first_red;
		while (next_red && next_red->type >= REDIRECT_IN
			&& next_red->type <= HEREDOC)
		{
			cur_red->left = cur_red->right;
			cur_red->right = NULL;
			cur_red = next_red;
			next_red = cur_red->right;
		}
		cmd->left = cur_red->left;
		*node = first_red;
		cur_red->left = cmd;
	}
	move_cmds(&(*node)->left);
	move_cmds(&(*node)->right);
} 

static bool	parse_token(char *line, t_token **token_list, char ***env)
{
	t_ast	*ast_root;
	t_token	*temp;

	ast_root = NULL;
	temp = NULL;
	if (!line)
		return (false);
	if (!lexer(line, token_list))
		return (false);
	free(line);
	//print_tokens(token_list);
	kobe_expander(*token_list, *env);
	temp = *token_list;
	if (validate_token_sequence(*token_list))
	{
		if ((ast_root = parse_ast(&temp)))
		{
		//	print_ast(ast_root, 0);
			move_cmds(&ast_root); /////////////
			set_root_ast(ast_root, ast_root);
			printf(BOLD_MAGENTA"\nAbstract Syntax Tree:\n"RESET);
		//	print_ast(ast_root, 0);
		//	printf(BOLD_MAGENTA"\noutput:\n"RESET);
			free_token_list(token_list);
			executor(ast_root, env);
		//	printf(BOLD_MAGENTA"end output\n"RESET);
			free_ast(ast_root);
		}
	}
	else
		return (false);
	return (true);
}

static void	excecute_test(char *line, t_token **token_list, char ***env)
{
	*token_list = NULL;
	if (line)
		parse_token(line, token_list, env);
}

/*	Checks paramters when running ./minishell
 *	Reads input and tokenizes it
 *	After tokenizing, parses the token_list
 *	Gets expanded and executed
 *	If debug = 0, run program normally
 *	If debug = 1, run program, when input == test
 *	--> runs tests
 */

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*token_list;
	char	**env;

	(void)argv;
	env = envp;
	if (!check_input(argc, envp))
		return (1);
	env = ft_copy_tab(envp);
	if (!env)
		return (print_error("Failure copying envp into env"), 1);
	if (isatty(STDIN_FILENO))
		setup_signals();
	while (1)
	{
		token_list = NULL;
		line = NULL;
		if (!(line = handle_line(env)))
		{
			printf("exit\n");
			exit(g_exit_status);
		}
		if (ft_strcmp(line, "exit") == 0)
			return (free(line), ft_free_tab(env), 0);
		if (DEBUG == 0)
		{
			if (!parse_token(line, &token_list, &env))
				return (free_token_list(&token_list), 1);
		}
		else if (DEBUG == 1)
		{
			if (ft_strcmp("test", line) == 0)
			{
				excecute_test(ft_strdup("> t1"), &token_list, &env);
				excecute_test(ft_strdup("cat > t1"), &token_list, &env);
				excecute_test(ft_strdup("cat > t1 > t2"), &token_list, &env);
				excecute_test(ft_strdup("> t1 cat > t2"), &token_list, &env);
			}
			else
				if (!parse_token(line, &token_list, &env))
					return (free_token_list(&token_list), 1);
		}
	}
	// rl_clear_line();
	return (g_exit_status);
}
