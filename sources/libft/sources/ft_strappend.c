#include "../libft.h"

/*
 *	Appends a single char to a string and returns new string
 */

char	*ft_strappend(char *str, char c)
{
	char	*new_str;
	size_t	len;

	new_str = NULL;
	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
		return (NULL);
	if (str)
		ft_memcpy(new_str, str, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}
