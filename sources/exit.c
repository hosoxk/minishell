
#include "minishell.h"

void	exit_clean(char *err_msg, t_ast *node, char **env)
{
	print_error(err_msg);
	if (node)
		free_ast(node->root);
	if (env)
		ft_free_tab(env);
	if (errno == ENOENT)
		exit(127);
	if (errno == EACCES)
		exit(126);
	exit(1);
}
