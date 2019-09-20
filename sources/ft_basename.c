/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_basename.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 20:45:07 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/20 23:14:37 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_avl_tree.h"
#include "ft_string.h"
#include "parser.h"
#include "sys_tools/sys_tools.h"

char		*ft_basename(const char *path)
{
	char	*it;

	it = ft_strrchr(path, '/');
	if (*(++it) == 0)
		return (ft_strdup("/")); // MALLOC DEFENSE HIGHER
	return (ft_strdup(it));
}

int			sh_check_f_b(char *name, ENV *envr)
{
	if (ft_avl_search(envr->funcs, name))
		return (1);
	else if (ft_avl_search(envr->builtns, name))
		return (2);
	return(0);
}

pid_t		set_pid_err(pid_t pid, char *path)
{
	const int perms = sys_touch_file(path);

	pid = !(perms & 0x8) ? -1 : 0; // NOT FOUND
	pid = !(perms & 0x1) && !(pid) ? -5 : pid; // NOT EXE
	pid = (perms & 0x40) && !(pid) ? -9 : pid; // IS DIR
	pid = !(perms & 0x80) && !(pid) ? -10 : pid; //? NOT REGULAR
	return (pid);
}

char		*sh_checkpathes(const char *cmd, char **pathes, pid_t *pid)
{
	int			i;
	size_t		len[2];
	char		*str;
	int			perms;

	i = -1;
	len[0] = ft_strlen(cmd);
	while (pathes[++i])
	{
		len[1] = ft_strlen(pathes[i]) + len[0] + 1;
		str = ft_strnew(len[1]);
		ft_strcat(str, pathes[i]);
		ft_strcat(str, "/");
		ft_strcat(str, cmd);
		*pid = set_pid_err(*pid, str);
		if (!(*pid))
			return (str);
		free(str);
	}
	return (0);
}

// ! CHECK FOR A DIRS
char		*sh_checkbins(const char *cmd, ENV *envr, pid_t *pid)
{
	DSTRING		*dstr;
	char		**pathes;
	char		*str;
	int			perms;

	if (((*cmd == '.' && *(cmd + 1) == '/') \
	|| *cmd == '/'))
	{
		*pid = set_pid_err(*pid, str);
		if (!(*pid))
			return (ft_strdup(cmd));
		else
			return (0);
	}
	if (!(dstr = env_get_variable("PATH", envr)))
		return (0);
	if (!(pathes = ft_strsplit(dstr->txt, ':')))
		return (0);
	str = sh_checkpathes(cmd, pathes, pid);
	dstr_del(&dstr);
	et_rm_warr(pathes);
	if (str)
		return (str);
	return (0);
}
