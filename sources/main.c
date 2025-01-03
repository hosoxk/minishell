/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:38:48 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/03 17:39:50 by kvanden-         ###   ########.fr       */
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

static char	*handle_line(char **env)
{
	char	*line;

	line = readline(get_prompt(env));
	if (!line)
		return (NULL);
	// add command to history
	if (*line)
		add_history(line);
	return (line);
}

static void parse_token(char *line, t_token *token_list, t_ast *ast_root, char ***env)
{
	if (!line)
		return ;
	(void)env;
	// tokenize input
	lexer(line, &token_list);
	free (line);
	kobe_expander(token_list, *env);
	print_tokens(&token_list);
	if (validate_token_sequence(token_list))
	{
		// parse tokens into AST
		if ((ast_root = parse_ast(&token_list)))
		{
			//expand_ast(ast_root, *env);
			executor(ast_root, env);
			printf(BOLD_MAGENTA"\nAbstract Syntax Tree:\n"RESET);
			print_ast(ast_root, 0);
		}
	}
}

void excecute_test(char *line, t_token **token_list, t_ast **ast_root, char ***env)
{
	if (line)
		parse_token(line, *token_list, *ast_root, env);
	*token_list = NULL;
	*ast_root = NULL;
}

void	handle_sigquit(int sig)
{
	(void)sig;

	// do nothing to ignore SIGQUIT
}

void	handle_sigint(int sig)
{
	(void)sig;

	g_exit_status = 130;
	printf("\n"); // new line for clean prompt ?? // TODO
	rl_on_new_line(); // resets new line to new rule
	rl_replace_line("", 0); // clean current line
	rl_redisplay(); // shows new line
}

static void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	char	*line;
	t_token	*token_list;
	t_ast	*ast_root;
	char	**env;
	
	env = envp;
	if (!check_input(argc, envp))
		return (1);
	env = ft_copy_tab(envp);
	if (!env)
		return (print_error("Failure copying envp into env"), 1);
	setup_signals();
	while (1)
	{
		token_list = NULL;
		ast_root = NULL;
		line = NULL;
		// read input
		if (!(line = handle_line(env)))
		{
			printf("exiting\n");
			exit(g_exit_status);
		}
		if (ft_strcmp(line, "exit") == 0)
			return (free(line), 0);
		// tokenize, create AST, expand, execute
		if (DEBUG == 0)
			parse_token(line, token_list, ast_root, &env);
		else if (DEBUG == 1)
		{
			if (ft_strcmp("test", line) == 0)
			{
				excecute_test(ft_strdup("ls -lR"), &token_list, &ast_root, &env);
				excecute_test(ft_strdup("echo \"hello\""), &token_list, &ast_root, &env);
			}
			else
				parse_token(line, token_list, ast_root, &env);
		}
		free_program(token_list, ast_root);
	}
	return (g_exit_status);
}
