#include "../minishell.h"

static void	allocate_args(char ***args, int *size)
{
	*size = ARG_SIZE;
	*args = malloc(sizeof (char *) * (*size));
	if (!*args)
		print_error("Error: failure allocating arguments array");
}

static void	reallocate_args(char ***args, int *size)
{
	int	old_size;

	old_size = *size * sizeof (char *);
	*size += ARG_SIZE;
	*args = ft_realloc(*args, old_size, *size * sizeof (char *));
	if (!*args)
		print_error("Error: failure reallocating arguments array");
}

void	add_argument(char ***args, int *size, int *count, const char *value)
{
	if (!value)
	{
		print_error("Error: attempting to add NULL argument");
		return ;
	}
	printf("Adding argument: %s (count = %d, size = %d)\n",
		value, *count, *size);
	if (!*args)
		allocate_args(args, size);
	else if (*count >= (*size - 1))
		reallocate_args(args, size);
	(*args)[*count] = ft_strdup(value);
	if (!(*args)[*count])
	{
		print_error("Error: ft_strdup failed in add_argument\n");
		return ;
	}
	(*count)++;
	(*args)[*count] = NULL;
	printf("Argument added succesfully. total count = %d\n", *count);
}
