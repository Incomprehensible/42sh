/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_type_bins_check.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 10:34:05 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/02 20:19:50 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_type.h"
#include <dirent.h>

static int			bltn_tconcat(char **path, char *name, DSTRING ***res,\
					DIR *dir)
{
	(*(*res)) = dstr_new(*path);
	if (*path)
		free(*path);
	dstr_insert_ch((*(*res)), '/', (*(*res))->strlen);
	dstr_insert_str((*(*res)), name, (*(*res))->strlen);
	closedir(dir);
	return (1);
}

static char			*bltn_type_initvars(int fl, int fl2, ENV *envp)
{
	if (fl2 == 1)
	{
		if (fl == 1 && (ft_avl_search(envp->globals, "PATH") != 0))
			return ((char *)ft_avl_search(envp->globals, "PATH")->content);
		else if (fl == 2 && (ft_avl_search(envp->locals, "PATH") != 0))
			return ((char *)ft_avl_search(envp->locals, "PATH")->content);
	}
	return (NULL);
}

static int			bltn_type_find(char *str, DSTRING **res, ENV *envp, int fl)
{
	char			*path;
	char			**spl;
	DIR				*dir;
	struct dirent	*entry;
	char			**spl_tmp;

	path = bltn_type_initvars(fl, 1, envp);
	spl_tmp = bltn_type_initspl(&spl, path);
	while (*spl)
	{
		if (!(dir = opendir(*spl)))
			return (bltn_type_freespl(&spl_tmp));
		while ((entry = readdir(dir)) != NULL)
		{
			if (ft_strequ(str, entry->d_name))
			{
				path = ft_strdup(*spl);
				bltn_type_freespl(&spl_tmp);
				return (bltn_tconcat(&path, (char *)entry->d_name, &res, dir));
			}
		}
		closedir(dir);
		spl++;
	}
	return (bltn_type_freespl(&spl_tmp));
}

static DSTRING		*bltn_type_chbins_parse(char *str, ENV *envp)
{
	DSTRING	*res;

	res = NULL;
	if (bltn_type_find(str, &res, envp, 1))
		return (res);
	else if (bltn_type_find(str, &res, envp, 2))
		return (res);
	return (res);
}

int					bltn_type_check_bins(char *str, ENV *env, DSTRING **val)
{
	t_darr	res;
	size_t	i;
	DSTRING	*dstr;

	i = (size_t)-1;
	res.allsize = 0;
	res.maxlen = 0;
	res.count = 0;
	dstr = dstr_new(str);
	env_get_bins_parsepath(&res, env, dstr);
	dstr_del(&dstr);
	while (++i < res.count)
	{
		if (ft_strequ(res.strings[i]->txt, str))
		{
			if ((*val))
				dstr_del(val);
			bltn_type_freedstrings(&res);
			(*val) = bltn_type_chbins_parse(str, env);
			return (1);
		}
	}
	bltn_type_freedstrings(&res);
	return (0);
}
