/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:36:00 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/08 16:00:42 by yde-rudd         ###   ########.fr       */
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
# include <sys/types.h>
# include <dirent.h>

# define BOLD_MAGENTA "\033[35m"
# define BOLD_RED "\033[31m"
# define BOLD_BLUE "\033[1;94m"
# define BOLD_GREEN "\033[32m"
# define RESET "\033[0m"

# define ARG_SIZE 8;

# ifndef DEBUG
#  define DEBUG 0
# endif

extern volatile int	g_exit_status;

/*
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
*/
typedef enum e_token_type
{
	WORD,
	PIPE,
	OR,
	AND,
	PARENTHESES_OPEN,
	PARENTHESES_CLOSE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	VARIABLE,
	QUOTED_STRING,
	DOUBLE_QUOTED_STRING,
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

/*
typedef struct s_ast
{
	t_token_type	type;
	char **args;         // array of arguments for commands
	char *file;          // file name for redirection
	struct s_ast *left;  // left child (for pipes and redirects)
	struct s_ast *right; // right child
	struct s_ast *root;  // for free in exution
	int fd_in;           // for heredocs
	int fd_out;          // for heredocs
}					t_ast;
*/
typedef struct s_ast
{
	t_token_type	type;
	char			**args;
	char			*file;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*root;
	int				fd_in;
	int				fd_out;
}					t_ast;


typedef struct s_tree
{
	t_token_type	type;
	struct s_tree	*left;
	struct s_tree	*right;
	t_token			*token_list;
}					t_tree;

void				set_root_ast(t_ast *node, t_ast *root);

// *** LEXER & HANDLER ***
bool				lexer(char *line, t_token **token_list);
void				handle_var(char **line, t_token **token_list);
bool				handle_redirect(char **line, t_token **token_list);
bool				handle_quoted_str(char **line, t_token **token_list);
bool				add_token_to_list(t_token **token_list, char *value,
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
void				attach_redirection_to_command(t_ast *command, t_ast *redir,
						bool is_prefix);
t_ast				*create_command_node(t_ast *command_node, t_token **tokens,
						int *size, int *count);

void free_token(t_token * token);
void free_token_tree(t_tree* tree);
void build_token_tree(t_tree **tree, t_token *token_list);
void execute_token_tree(t_tree *tree, char ***env);
void execute_sub_commands(t_token **token_list, char ***env);
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
void				exit_clean(t_ast *node, char **env, int exit_status);

// *** EXPANDER ***
void				expand_ast(t_ast *node, char **env);
void				kobe_expander(t_token *token_list, char **env);
bool 				expand_wildcard(t_token *token);

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

bool				init_heredoc(t_ast *ast_root);

// *** ENVIRONMENT ***
char				*getenv_stript(char *name, char **env);
int					getenv_index(char *name, char **env);
void				update_env(char *name, char *value, char **env);
char				*get_env_value(t_token *token, int index, int len,
						char **env);

// *** UTILS ***
char				*get_prompt(char **env);
char				*handle_line(char **env);

// *** pids ***
pid_t				*get_pid_list(t_ast *ast_root);
void				set_pid(pid_t *pids, pid_t pid);
int					get_len_pids(pid_t *pids);


// *** parentheses ***

bool handle_parentheses_tree(t_tree **tree, t_token *token_list);
void execute_token_tree(t_tree *tree, char ***env);
void build_token_tree(t_tree **tree, t_token *token_list);
t_token *split_before(t_token *list, t_token *target);
t_token *find_operator(t_token *token, t_token_type type);
t_tree *get_token_node(t_token_type type, t_token *token_list);

#endif
