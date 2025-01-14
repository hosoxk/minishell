/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 09:46:06 by kvanden-          #+#    #+#             */
/*   Updated: 2025/01/14 09:46:06 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	match_pattern(const char *name, const char *pattern)
{
	if (name[0] == '.' && !strchr(pattern, '.'))
		return (false);
	while (*name || *pattern)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			if (!*pattern)
				return (true);
			while (*name && !match_pattern(name, pattern))
				name++;
			return (*name != '\0');
		}
		else if (*pattern == *name || *pattern == '?')
		{
			pattern++;
			name++;
		}
		else
			return (false);
	}
	return (*name == '\0' && *pattern == '\0');
}

static t_token	*insort_wildcard(const char *name, bool first)
{
	static t_token	*new_list = NULL;
	t_token			*new_token;
	t_token			*pt;

	if (first)
		new_list = NULL;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (print_error("malloc"), NULL);
	new_token->value = ft_strdup(name);
	if (!new_token->value)
		return (free(new_token), free_token_list(&new_list),
			print_error("malloc"), NULL);
	new_token->type = WORD;
	new_token->next = NULL;
	if (!new_list)
		return (new_list = new_token, new_list);
	else
	{
		pt = new_list;
		while (pt->next)
			pt = pt->next;
		pt->next = new_token;
	}
	return (new_list);
}

static t_token	*get_list(t_token *token, DIR *dir)
{
	struct dirent	*entry;
	t_token			*list;
	bool			first;

	first = true;
	list = NULL;
	entry = readdir(dir);
	while (entry)
	{
		if (match_pattern(entry->d_name, token->value))
		{
			list = insort_wildcard(entry->d_name, first);
			if (!list)
				return (NULL);
			first = false;
		}
		entry = readdir(dir);
	}
	return (list);
}

bool	expand_wildcard(t_token *token)
{
	DIR		*dir;
	t_token	*list;
	t_token	*next;
	
	dir = opendir(".");
	if (!dir)
		return (perror("opendir"), false);
	list = get_list(token, dir);
	closedir(dir);
	if (!list)
		return (false);
	sort_linkt_list(list);
	free(token->value);
	token->value = list->value;
	next = token->next;
	token->next = list->next;
	free(list);
	while (token->next)
		token = token->next;
	token->next = next;
	return (true);
}
