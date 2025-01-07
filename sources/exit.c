
#include "minishell.h"

void	exit_clean(char *err_msg, t_ast *node, char **env, int exit_status)
{
	print_error(err_msg);
	if (node)
		free_ast(node->root);
	if (env)
		ft_free_tab(env);
	if (exit_status != -1)
		exit(exit_status);
	if (errno == ENOENT)
		exit(127);
	if (errno == EACCES)
		exit(126);
	exit(1);
}
