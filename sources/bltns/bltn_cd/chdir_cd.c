/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 22:53:19 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/06 23:14:26 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"
#include <unistd.h>
#include <limits.h>

static DSTRING	*cd_path_maker(char **lpath)
{
	size_t			i;
	ssize_t			j;
	t_dyn_string	*path;

	i = -1;
	j = 0;
	path = dstr_new("");
	while (lpath[++i])
	{
		dstr_insert_ch(path, '/', path->strlen);
		if (ft_strequ("..", lpath[i]) && path->strlen)
			dstr_slice_del(&path, j, path->strlen);
		else
			dstr_insert_str(path, lpath[i], path->strlen);
		j = path->strlen;
		while (j && path->txt[j] != '/')
			--j;
	}
	if (path->strlen == 0)
		dstr_insert_ch(path, '/', path->strlen);
	return (path);
}

int				cd_logic(DSTRING **path)
{
	char		**lpath;

	if ((*path)->strlen == 0)
		return (0);
	if ((*path)->txt[0] != '/' && (dstr_insert_ch(*path, '/', 0) || 1))
		dstr_insert_dstr(*path, g_pwd, 0);
	if (!(lpath = ft_strsplit((*path)->txt, '/')))
		sys_fatal_memerr("cd: ALLOCATION FAILS");
	dstr_del(path);
	if (!(*path = cd_path_maker(lpath)))
		sys_fatal_memerr("cd: ALLOCATION FAILS");
	dstr_del(&g_oldpwd);
	g_oldpwd = g_pwd;
	g_pwd = dstr_new((*path)->txt);
	chdir(g_pwd->txt);
	et_rm_warr(lpath);
	return (0);
}

int				cd_phpath(DSTRING **path)
{
	char		cwd[PATH_MAX];

	if ((*path)->strlen == 0)
		return (0);
	dstr_del(&g_oldpwd);
	g_oldpwd = g_pwd;
	chdir((*path)->txt);
	ft_bzero(cwd, PATH_MAX);
	getcwd(cwd, PATH_MAX);
	g_pwd = dstr_new(cwd);
	return (0);
}
