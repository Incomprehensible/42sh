/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_vars_gets.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 13:02:20 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/17 21:40:47 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_vars.h"

static void	sh_lcmds_cmp_name(t_darr *res, int namelen)
{
	if ((size_t)namelen > res->maxlen)
		(res)->maxlen = (size_t)namelen;
	(res)->allsize += (size_t)namelen;
}

static int	sh_vars_parsepath(t_darr *res, char *path)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**spl;
	int				i;

	i = -1;
	if ((spl = ft_strsplit(path, ':')) == 0)
		return (0);
	while (*spl)
	{
		if (!(dir = opendir(*spl)))
			return (0);
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_name[0] == '.')
				continue ;
			(res)->strings[++i] = dstr_new((char *)entry->d_name);
			(res)->count++;
			sh_lcmds_cmp_name(res, (int)entry->d_namlen);
		}
		closedir(dir);
		spl++;
	}
	(res)->strings[++i] = NULL;
	return (1);
}

t_darr		get_list_cmds(t_envp *envp)
{
	t_darr	res;

	res.allsize = 0;
	res.maxlen = 0;
	res.maxlen = 0;
	res.count = 0;
	if (ft_avl_search(envp->global, "PATH") != 0)
	{
		if ((sh_vars_parsepath(&res,\
			(char *)ft_avl_search(envp->global, "PATH")->content)) == 0)
			return (res);
	}
	else if (ft_avl_search(envp->local, "PATH") != 0)
	{
		if ((sh_vars_parsepath(&res,\
			(char *)ft_avl_search(envp->local, "PATH")->content)) == 0)
			return (res);
	}
	else
		return (res);
	return (res);
}
