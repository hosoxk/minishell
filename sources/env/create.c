#include "../minishell.h"

static bool incroment_shell_level(char ***env)
{
    int n;
    char *org_number;
    char *mod_number;

    org_number = getenv_stript("SHLVL", *env);
    if (!org_number)
        n = 1;
    else
        n = ft_atoi(org_number) + 1;
    mod_number = ft_itoa(n);
    if (!mod_number)
    {
        print_error("failure convorting shell level");
        return (false);
    }
	if (!update_env("SHLVL", mod_number, env))
    {
        free(mod_number);
        return (false);
    }
    free(mod_number);
    return (true);
}

char **create_env(char **envp)
{
    char	**env;
    char	cwd[PATH_MAX];

	env = ft_copy_tab(envp);
	if (!env)
		return (print_error("Failure copying envp into env"), NULL);
    if (!incroment_shell_level(&env))
    {
        return (ft_free_tab(env), NULL);
    }
    if (!getcwd(cwd, sizeof(cwd)))
    {
        ft_free_tab(env);
        g_exit_status = 1;
        perror("getcwd");
		return (NULL);
    }
	if (!update_env("PWD", cwd, &env))
        return (ft_free_tab(env), NULL);
    return (env);
}
