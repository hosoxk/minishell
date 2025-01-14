
#include "../minishell.h"
 
t_tree *get_token_node(t_token_type type, t_token *token_list)
{
    t_tree *node;

    node = malloc(sizeof(t_tree));
    if (!node)
        return (NULL);
    node->left = NULL;
    node->right = NULL;
    node->token_list = token_list;
    node->type = type;
    return (node);
}

t_token *find_operator(t_token *token, t_token_type type)
{
    int depth;
    
    depth = 0;
    while (token) 
    {
        if (token->type == PARENTHESES_OPEN)
            depth++;
        if (token->type == PARENTHESES_CLOSE) 
            depth--;
        if ((depth == 0 || type == PARENTHESES_OPEN) && token->type == type)
            return (token);
        token = token->next;
    }
    return (NULL);
}

t_token *split_before(t_token *list, t_token *target)
{
    t_token *current;
    
    current = list;
    while (current && current->next != target)
        current = current->next;
    if (current)
        current->next = NULL;
    return (list);
}
