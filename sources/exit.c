
#include "minishell.h"

void	exit_clean(char *err_msg, t_ast *ast_root, char **env)
{
	print_error(err_msg);
	if (ast_root)
		free_ast(ast_root);
	if (env)
		ft_free_tab(env);
	exit(1);
}
