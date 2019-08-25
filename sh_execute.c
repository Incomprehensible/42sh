/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 15:14:47 by hgranule          #+#    #+#             */
/*   Updated: 2019/07/21 21:53:31 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_nuclr.h"

extern int			(*g_builtin_func[])(char **);
extern char const	*g_builtins[];

static int			sh_launch(char *const *cmds)
{
	pid_t	cpid;
	pid_t	wpid;
	int		status;

	cpid = fork();
	if (cpid == 0 && (execve(cmds[0], cmds, g_envp) < 0))
	{
		ft_putendl("fail#1");
		sh_trmnt_cleanup();
		exit(1);
	}
	else if (cpid > 0)
	{
		while ((wpid = waitpid(cpid, &status, WUNTRACED)) || 1)
			if (WIFEXITED(status) || WIFSIGNALED(status))
				break ;
	}
	else if (cpid < 0)
	{
		ft_putendl("fail#2");
	}
	return (1);
}

int					sh_execute(char **cmds)
{
	int		i;
	char	*path;

	if (cmds[0] == 0) // empty command;
		return (1);
	i = -1;
	while (g_builtins[++i]) // CHECK FOR A BUILTN
	{
		if (ft_strequ(g_builtins[i], cmds[0]) > 0)
			return (g_builtin_func[i]((char **)cmds));
	}
	if ((path = sh_checkbins(cmds[0])) != 0) // CHECK THE PATHES
	{
		free(cmds[0]);
		cmds[0] = path;
	}
	// IF PATH FAILES CHECK THE ACCESS
	return (sh_launch(cmds)); // LAUNCH THE COMMAND
}
