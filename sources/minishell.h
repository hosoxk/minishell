/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/12/16 14:36:00 by yde-rudd          #+#    #+#             */
/*   Updated: 2025/01/08 16:00:42 by yde-rudd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

# define BOLD_MAGENTA "\033[35m"
# define BOLD_RED "\033[31m"
# define BOLD_BLUE "\033[1;94m"
# define BOLD_GREEN "\033[32m"
# define RESET "\033[0m"

# define ARG_SIZE 8;

# ifndef DEBUG
#  define DEBUG 0
# endif

extern volatile int			g_exit_status;

typedef enum e_token_type	t_token_type;
typedef enum e_command_type	t_command_type;
typedef struct s_free_data	t_free_data;
typedef struct s_token		t_token;
typedef struct s_ast		t_ast;
typedef struct s_token_tree	t_token_tree;

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
}	t_token_type;

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
}	t_command_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_ast
{
	t_token_type	type;
	char			**args;
	char			*file;
	struct s_ast	*left;
	struct s_ast	*right;
	t_free_data		*free_data;
}	t_ast;

typedef struct s_parse_vars
{
	t_ast	*left_node;
	t_ast	*redir_node;
	t_ast	*current_command;
	int		size;
	int		count;
	bool	is_prefix;
}	t_parse_vars;

typedef struct s_token_tree
{
	t_token_type		type;
	struct s_token_tree	*left;
	struct s_token_tree	*right;
	t_token				*token_list;
}	t_token_tree;

typedef struct s_free_data
{
	t_ast			*root;
	t_token_tree	*token_tree;
	int				fd_in;
	int				fd_out;
	int				*fds;
}	t_free_data;

char			*get_next_line(int fd);

void			set_data_to_ast(t_ast *node, t_free_data *data);
t_ast			*get_ast(t_token_tree *tree, t_free_data *data, \
	t_token_tree *token_tree_root);

// *** LEXER & HANDLER ***
bool			lexer(char *line, t_token **token_list);
void			handle_var(char **line, t_token **token_list);
bool			handle_redirect(char **line, t_token **token_list);
bool			handle_quoted_str(char **line, t_token **token_list);
bool			handle_pipe(char **line, t_token **token_list);
bool			handle_parentheses(char **line, t_token **token_list);
bool			handle_ampersand(char **line, t_token **token_list);
bool			add_token_to_list(t_token **token_list, char *value,
					t_token_type type);
bool			validate_token_sequence(t_token *tokens);
bool			is_valid_command(t_token *token);

// *** PARSER ***
t_ast			*parse_ast(t_token **tokens);
void			add_argument(char ***args, int *size, int *count, \
					const char *value);
t_ast			*create_ast_node(t_token_type type);
t_ast			*create_pipe_node(t_ast *left_node, t_token **tokens);
t_ast			*create_redirection_node(t_token **tokens);
void			attach_redirection_to_command(t_ast *command, t_ast *redir,
					bool is_prefix);
t_ast			*create_command_node(t_ast *command_node, t_token **tokens, \
					int *size, int *count);

void			free_token(t_token *token);
void			free_token_tree(t_token_tree *tree);
void			build_token_tree(t_token_tree **tree, t_token *token_list);
bool			execute_token_tree(t_token_tree *tree, char ***env, \
					t_token_tree *root);
bool			execute_sub_commands(t_token_tree *tree, char ***env, \
					t_token_tree *root);
// *** SIGNALS ***
void			setup_signals(void);

// *** UTILS ***
void			exit_status(int exit_status);
bool			is_special_case(char c);
void			print_tokens(t_token **token_list);
void			print_ast(t_ast *node, int depth);
void			print_error(char *str);

// *** CLEAN ***
void			free_token_list(t_token **token_list);
void			free_ast(t_ast *node);
void			free_env(char **env);
void			free_program(t_token *token_list, t_ast *ast_root, char **env);
void			exit_clean(t_ast *node, char **env, int exit_status);

// *** EXPANDER ***
void			expand_ast(t_ast *node, char **env);
bool			expander(t_token *token_list, char **env);
bool			expand_wildcard(t_token *token);
void			sort_linkt_list(t_token *list);

// *** EXECUTION ***
bool			execute(t_ast *ast_root, char ***env, pid_t *pids, \
					bool is_first);
bool			executor(t_ast *ast_root, char ***env);
void			execute_build_in_cmd(char **argv, char **env);
bool			execute_custom_cmd_after_fork(char **argv, char ***env);
bool			execute_custom_cmd(t_ast *ast_root, char ***env);
bool			do_pipe(t_ast *ast_root, char ***env, pid_t *pids);
bool			do_redirection(t_ast *ast_root, char ***env, pid_t *pids,
					bool is_first);
bool			init_heredoc(t_ast *ast_root, char **env, pid_t *pids);
void			my_echo(char **argv);
void			my_env(char **env);
void			export(char ***env, char **argv);
void			unset(char ***env, char **argv);
void			my_cd(char **env, char **argv);
void			my_pwd(void);

// *** ENVIRONMENT ***
char			*getenv_stript(char *name, char **env);
int				getenv_index(char *name, char **env);
void			update_env(char *name, char *value, char **env);
char			*get_env_value(t_token *token, int index, int len, char **env);

// *** UTILS ***
char			*get_line(char **env);

// *** pids ***
pid_t			*get_pid_list(t_ast *ast_root);
void			set_pid(pid_t *pids, pid_t pid);
int				get_len_pids(pid_t *pids);

// *** parentheses ***

bool			handle_parentheses_tree(t_token_tree **tree, \
					t_token *token_list);
void			build_token_tree(t_token_tree **tree, t_token *token_list);
t_token			*split_before(t_token *list, t_token *target);
t_token			*find_operator(t_token *token, t_token_type type);
t_token_tree	*get_token_node(t_token_type type, t_token *token_list);
t_token			*find_last_operator(t_token *token_list);

// *** FDS ***

int				*get_fds(t_ast *tree);
void			clear_fds(int *fds);
void			add_fd(t_ast *tree, int fd);
void			pop_fd(t_ast *tree);

#endif
