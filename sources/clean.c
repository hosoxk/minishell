#include "minishell.h"

void	free_token_list(t_token **token_list)
{
	t_token	*tmp;

	while (*token_list)
	{
		tmp = (*token_list)->next;
		free((*token_list)->value);
		free(*token_list);
		*token_list = tmp;
	}
	*token_list = NULL;
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->args)
	{
		for (int i = 0; node->args[i]; i++)
			free(node->args[i]);
		free(node->args);
	}
	if (node->file)
		free(node->file);
	free(node);
}
