#include "../minishell.h"

int	echo(char **args)
{
	int	i;
	bool	newline;

	i = 1;
	newline = 1; //by default, echo should print a newline
	if (args[1] && ft_strcmp(args[1], "-n"))
	{
		newline = 0;
		i = 2;
	}
	while (args[i] != '\0')
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}
