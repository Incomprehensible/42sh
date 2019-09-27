/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_bins_parsepath.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:11:22 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/24 18:56:08 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <dirent.h>
#include "sys_tools/sys_tools.h"

static void	env_get_bins_setfpath(DSTRING **fpath, char *dir, char *filename)
{
	(*fpath) = dstr_new(dir);
	dstr_insert_ch((*fpath), '/', (*fpath)->strlen);
	dstr_insert_str((*fpath), filename, (*fpath)->strlen);
}

static int	env_get_bins_freespl(char ***spl)
{
	int		i;

	i = -1;
	if (*spl)
	{
		while ((*spl)[++i])
			free((*spl)[i]);
		free((*spl));
	}
	return (1);
}

static void	env_get_bins_setstring(t_darr *res, struct dirent *entry)
{
	(res)->strings[(res)->count] = dstr_new((char *)entry->d_name);
	(res)->count++;
	env_get_bins_cmp_name(res, (int)entry->d_namlen);
}

static int	env_get_bins_parse(t_darr *res, char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	DSTRING			*fpath;
	char			**spl_tmp;
	char			**spl;

	if ((spl = ft_strsplit(path, ':')) == 0)
		return (0);
	spl_tmp = spl;
	while (*spl)
	{
		if (!(dir = opendir(*spl)))
			return (0);
		while ((entry = readdir(dir)) != NULL)
		{
			env_get_bins_setfpath(&fpath, *spl, (char *)entry->d_name);
			if (!sys_is_ex_bin(fpath->txt) && env_get_bins_unq(res, fpath->txt))
				env_get_bins_setstring(res, entry);
			dstr_del(&fpath);
		}
		closedir(dir);
		spl++;
	}
	(res)->strings[(res)->count] = NULL;
	return (env_get_bins_freespl(&spl_tmp));
}

void		env_get_bins_parsepath(t_darr *res, ENV *envp)
{
	if (ft_avl_search(envp->globals, "PATH") != 0)
		env_get_bins_parse(res,\
			(char *)ft_avl_search(envp->globals, "PATH")->content);
	if (ft_avl_search(envp->locals, "PATH") != 0)
		env_get_bins_parse(res,\
			(char *)ft_avl_search(envp->locals, "PATH")->content);
}
