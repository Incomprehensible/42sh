/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_basename.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 20:45:07 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/16 20:52:37 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_avl_tree.h"
#include "ft_string.h"
#include "parser.h"
#include "sys_tools/sys_tools.h"

#include "bltn_hash.h"

char		*ft_basename(const char *path)
{
	char	*it;

	it = ft_strrchr(path, '/');
	if (*(++it) == 0)
		return (ft_strdup("/"));
	return (ft_strdup(it));
}

int			sh_check_f_b(char *name, ENV *envr)
{
	if (ft_avl_search(envr->funcs, name))
		return (1);
	else if (ft_avl_search(envr->builtns, name))
		return (2);
	return (0);
}

pid_t		set_pid_err(pid_t pid, char *path)
{
	const int code = sys_is_ex_bin(path);

	pid = code == -1 ? -1 : 0;
	pid = code == -3 ? -5 : pid;
	pid = code == -2 ? -9 : pid;
	pid = code == -4 ? -10 : pid;
	return (pid);
}

char		*sh_checkpathes(const char *cmd, char **pathes, pid_t *pid)
{
	int			i;
	size_t		len[2];
	char		*str;

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

char		*sh_checkbins(const char *cmd, ENV *envr, pid_t *pid)
{
	char			*str;
	t_hentr			entry;

	if (((*cmd == '.' && *(cmd + 1) == '/') || *cmd == '/'))
		return (path_check(cmd, envr, pid));
	if ((str = cache_check(cmd, envr, pid)))
		return (str);
	if ((str = tree_check(cmd, envr, pid)))
	{
		entry.hits = 1;
		ft_strncpy(entry.path, str, PATH_MAX);
		ht_add(envr->htid, (char *const)cmd, &entry, sizeof(t_hentr));
	}
	return (str);
}
