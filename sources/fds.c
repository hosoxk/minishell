
#include "minishell.h"

static int get_len(t_ast *tree)
{
	int i;

	i = 0;
	if (!tree)
		return (0);
	if (tree->type == PIPE)
		return (4);
	i += get_len(tree->left);
	i += get_len(tree->right);
	return (i);
}

int *get_fds(t_ast *tree)
{
    int len;
    int *fds;

    len = get_len(tree);
    if (len == 0)
        len = 1;
    fds = malloc(sizeof(int) * len);
    if (!fds)
        return (NULL);
    ft_memset(fds, -1, sizeof(int) * len);
    return (fds);
}

void clear_fds(int *fds)
{
    int *pt;

    if (!fds)
        return ;
    pt = fds;
    while (*pt != -1)
    {
        close(*pt);
        pt++;
    }
    free(fds);
}

void add_fd(t_ast *tree, int fd)
{
    int *fds;

    if (!tree->free_data->fds)
        return ;
    fds = tree->free_data->fds;    
    while (*fds != -1)
        fds++;
    *fds = fd;
}

void pop_fd(t_ast *tree)
{
    int *fds;

    fds = tree->free_data->fds;
    if (!fds)
        return ;
    while (*fds != -1)
        fds++;
    fds--;
    close(*fds);
    *fds = -1;
}
