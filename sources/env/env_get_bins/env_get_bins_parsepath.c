/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_bins_parsepath.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:11:22 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/03 00:32:58 by hgranule         ###   ########.fr       */
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

static int	env_get_bins_freespl(char ***spl, DIR *dir)
{
	int		i;

	i = -1;
	if (*spl)
	{
		while ((*spl)[++i])
			free((*spl)[i]);
		free((*spl));
	}
	return (dir ? 1 : 0);
}

static void	env_get_bins_setstring(t_darr *res, struct dirent *entry)
{
	(res)->strings[(res)->count] = dstr_new((char *)entry->d_name);
	(res)->count++;
	env_get_bins_cmp_name(res, (int)entry->d_namlen);
}

static int	env_get_bins_parse(t_darr *res, DSTRING *nm, char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	DSTRING			*fpath;
	char			**spl;
	size_t			i;

	if ((spl = ft_strsplit(path, ':')) == 0)
		return (0);
	i = -1;
	while (spl[++i] && (dir = opendir(spl[i])))
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (ft_strnequ(entry->d_name, nm->txt, nm->strlen) == 0)
				continue ;
			env_get_bins_setfpath(&fpath, spl[i], (char *)entry->d_name);
			if (!sys_is_ex_bin(fpath->txt) && env_get_bins_unq(res, fpath->txt))
				env_get_bins_setstring(res, entry);
			dstr_del(&fpath);
		}
		closedir(dir);
	}
	res->strings[res->count] = NULL;
	return (env_get_bins_freespl(&spl, dir));
}

void		env_get_bins_parsepath(t_darr *res, ENV *envp, DSTRING *sub)
{
	if (ft_avl_search(envp->globals, "PATH") != 0)
		env_get_bins_parse(res, sub,\
			(char *)ft_avl_search(envp->globals, "PATH")->content);
	if (ft_avl_search(envp->locals, "PATH") != 0)
		env_get_bins_parse(res, sub,\
			(char *)ft_avl_search(envp->locals, "PATH")->content);
}
