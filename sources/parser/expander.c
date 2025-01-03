#include "../minishell"

static char	*get_var_name(char **str)
{
	char	*start;

	start = ++(*str);
	while (**str && (ft_isalnum(**str) || **str == '_'))
		(*str)++;
	return (ft_substr(start, 0, *str - start);
}

static char	*getenv_from_env(const char *var_name, char **env)
{
	size_t	var_len;
	int	i;

	if (!var_name || !env)
		return (NULL);
	var_len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (ft_strcmp(env[i], var_name, var_len) == 0 && env[i][var_len] == '=')
			return (env[i] + var_len + 1);
		i++;
	}
	return (NULL);
}

static void	expand_var(char **expanded, char **str, char **env)
{
	char	*var_name;
	char	*value;
	char	*temp;

	var_name = get_var_name(str);
	value = getenv_from_env(var_name, env);
	temp = *expanded;
	*expanded = ft_strjoin(*expanded, value ? value : "");
	free(temp);
	free(var_name);
}

static void	expand_tilde(char **expanded, char **str)
{
	char	*temp;

	temp = *expanded;
	*expanded = ft_strjoin(*expanded, getenv("HOME"));
	free(temp);
	(*str)++;
}

static void	expand_s_quote(char **expanded, char **str)
{
	char	*start;
	char	*temp;
	char	*literal;

	start = ++(*str);
	while (**str && **str != '\0')
		(*str)++;
	literal = ft_substr(start, 0, *str - start);
	temp = *expanded;
	*expanded = ft_strjoin(*expanded, literal);
	free(temp);
	free(literal);
	if (**str == '\'')
		(*str)++;
}

static void	expand_d_quote(char **expanded, char **str, char **env)
{
	char	*start;
	char	*temp;

	start = ++(*str);
	while (**str && **str != '\"')
	{
		if (**str == '$')
			expand_var(expanded, str, env);
		else
		{
			temp = *expanded;
			*expanded = ft_strappend(*expanded, **str);
			free(temp);
			(*str)++;
		}
	}
	if (**str == '\"')
		(*str)++;
}

static char	*expand_string(char *str, char **env)
{
	char	*expanded;
	char	*temp;

	expanded = ft_strdup("");
	temp = NULL;
	while (*str)
	{
		if (*str == '$')
			expand_var(&expanded, &str, env);
		else if (*str == '~')
			expand_tilde(&expanded, &str);
		else if (*str == '\'')
			expand_s_quote(&expanded, &str);
		else if (*str == '\"')
			expand_d_quote(&expanded, &str, env);
		else
		{
			temp = expanded;
			expanded = ft_strappend(expanded, *str);
			free(temp);
			str++;
		}
	}
	return (expanded);
}

static void	expand_args(char **args, char **env)
{
	int	i;

	i = 0;
	while (args[i++])
		args[i] = expand_string(args[i], env);
}

void	expand_ast(t_ast *node, char **env)
{
	if (!node)
		return ;
	if (node->type == WORD || node->type ==  QUOTED_STRING
		|| node->type == DOUBLE_QUOTED_STRING)
		if (node->args)
			expand_args(node->args, env);
	else if (node->type == APPEND || node->type == REDIRECT_IN
		|| node->type == REDIRECT_OUT)
		if (node->file)
			node->file = expand_string(node->file, env);
	else if (node->type == HEREDOC)
		if (node->file)
			node->file = expand_heredoc(node->file, env);
	expand_ast(node->left, env);
	expand_ast(node->right, env);
}
