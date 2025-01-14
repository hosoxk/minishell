#include "../minishell.h"

void	set_data_to_ast(t_ast *node, t_free_data *data)
{
	if (!node)
		return ;
	set_data_to_ast(node->left, data);
	set_data_to_ast(node->right, data);
	node->free_data = data;
}

static t_ast	*get_next(t_ast *cur_red, t_ast *next_red)
{
	cur_red->left = cur_red->right;
	cur_red->right = NULL;
	cur_red = next_red;
	return (cur_red->right);
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
			next_red = get_next(cur_red, next_red);
		cmd->left = cur_red->left;
		*node = first_red;
		cur_red->left = cmd;
	}
	move_cmds(&(*node)->left);
	move_cmds(&(*node)->right);
}



t_ast	*get_ast(t_token_tree *tree, t_free_data *data,
	t_token_tree *token_tree_root)
{
	t_ast	*root;
	t_token	*temp;

	temp = tree->token_list;
	root = parse_ast(&temp);
	if (!root)
		return (NULL);
	move_cmds(&root);
	data->root = root;
	data->token_tree = token_tree_root;
	data->fd_in = dup(STDIN_FILENO);
	data->fd_out = dup(STDOUT_FILENO);
	data->fds = get_fds(root);
	if (!data->fds)
		return (free_ast(root), NULL);
	set_data_to_ast(root, data);
	return (root);
}
