/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:36:00 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/07 16:57:19 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>

# define BOLD_MAGENTA "\033[35m"
# define BOLD_RED "\033[31m"
# define BOLD_BLUE "\033[1;94m"
# define BOLD_GREEN "\033[32m"
# define RESET "\033[0m"

# ifndef DEBUG
#  define DEBUG 0
# endif

# define ARG_SIZE 8;

extern volatile int	g_exit_status;

typedef enum e_token_type
{
	WORD,
	PIPE,                 // ||
	REDIRECT_IN,          // <
	REDIRECT_OUT,         // >
	APPEND,               // >>
	HEREDOC,              // <<
	VARIABLE,             // $VAR
	QUOTED_STRING,        // '...'
	DOUBLE_QUOTED_STRING, // "..."
}					t_token_type;

typedef enum e_command_type
{
	CMD_NONE = -1,
	CMD_ECHO,
	CMD_CD,
	CMD_PWD,
	CMD_EXPORT,
	CMD_UNSET,
	CMD_ENV,
	CMD_EXIT
}					t_command_type;

typedef struct s_token
{
	char			*value;

	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef struct s_ast
{
	t_token_type	type;
	char **args;         // array of arguments for commands
	char *file;          // file name for redirection
	struct s_ast *left;  // left child (for pipes and redirects)
	struct s_ast *right; // right child
	struct s_ast *root;  // for free in exution
}					t_ast;

void				set_root_ast(t_ast *node, t_ast *root);

// *** LEXER & HANDLER ***
void				lexer(char *line, t_token **token_list);
void				handle_var(char **line, t_token **token_list);
void				handle_redirect(char **line, t_token **token_list);
void				handle_quoted_str(char **line, t_token **token_list);
void				add_token_to_list(t_token **token_list, char *value,
						t_token_type type);
bool				validate_token_sequence(t_token *tokens);
bool				is_valid_command(t_token *token);

// *** PARSER ***
t_ast				*parse_ast(t_token **tokens);
void				add_argument(char ***args, int *size, int *count,
						const char *value);
t_ast				*create_ast_node(t_token_type type);
t_ast				*create_pipe_node(t_ast *left_node, t_token **tokens);
t_ast				*create_redirection_node(t_token **tokens);
void	attach_redirection_to_command(t_ast *command,
									t_token **tokens);
t_ast				*create_command_node(t_ast *command_node, t_token **tokens,
						int *size, int *count);

// *** SIGNALS ***
void				setup_signals(void);

// *** UTILS ***
bool				is_special_case(char c);
void				print_tokens(t_token **token_list);
void				print_ast(t_ast *node, int depth);
void				print_error(char *str);

// *** CLEAN ***
void				free_token_list(t_token **token_list);
void				free_ast(t_ast *node);
void				free_program(t_token *token_list, t_ast *ast_root);
void	exit_clean(t_ast *node, char **env, int exit_status);

// *** EXPANDER ***
void				expand_ast(t_ast *node, char **env);
void				kobe_expander(t_token *token_list, char **env);

// *** EXECUTION ***
void				execute(t_ast *ast_root, char ***env, pid_t *pids,
						bool is_first);
void				executor(t_ast *ast_root, char ***env);
void				execute_build_in_cmd(char *name, char **argv, char **env);
bool				execute_custom_cmd_after_fork(char *name, char **argv,
						char ***env, t_command_type type);
bool				execute_custom_cmd(t_ast *ast_root, char ***env);

void				do_pipe(t_ast *ast_root, char ***env, pid_t *pids);
void				do_redirection(t_ast *ast_root, char ***env, pid_t *pids,
						bool is_first);

void				my_echo(char **argv);
void				my_env(char **env);
void				export(char ***env, char **argv);
void				unset(char ***env, char **argv);
void				my_cd(char **env, char **argv);
void				my_pwd(void);

void				init_heredoc(t_ast *ast_root);

// *** ENVIRONMENT ***
char				*getenv_stript(char *name, char **env);
int					getenv_index(char *name, char **env);
void				update_env(char *name, char *value, char **env);
char				*get_env_value(t_token *token, int index, int len,
						char **env);

// *** UTILS ***
char				*get_prompt(char **env);

// *** pids ***
pid_t				*get_pid_list(t_ast *ast_root);
void				set_pid(pid_t *pids, pid_t pid);
int					get_len_pids(pid_t *pids);

#endif
