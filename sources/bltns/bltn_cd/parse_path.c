/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 20:49:27 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/08 19:07:54 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"

static int		cd_check_path(int mode, DSTRING *path)
{
	int			perms;

	perms = sys_touch_file(path->txt);
	if (!(perms & SYS_TCH_F))
		return (CD_MODE_ERROR_NE);
	if ((perms & SYS_TCH_TYP) != SYS_TCH_DIR)
		return (CD_MODE_ERROR_ND);
	if (!(perms & SYS_TCH_X))
		return (CD_MODE_ERROR_PD);
	return (mode);
}

DSTRING			*cdpath_get_path(DSTRING *cdpath, char *arg)
{
	DSTRING		*item;
	size_t		i;
	char		**dirs;

	if (!access(arg, F_OK | X_OK))
		return (dstr_new(arg));
	if (!(dirs = ft_strsplit(cdpath->txt, ':')))
		return (NULL);
	i = -1;
	item = NULL;
	while (dirs[++i])
	{
		if (!(item = dstr_new(dirs[i])))
			sys_fatal_memerr("ALLOCATION FAILS CDPATH");
		dstr_insert_ch(item, '/', item->strlen);
		dstr_insert_str(item, arg, item->strlen);
		if ((sys_touch_file(item->txt) & (SYS_TCH_DIR | SYS_TCH_F))\
		== (SYS_TCH_F | SYS_TCH_DIR))
			break ;
		dstr_del(&item);
	}
	et_rm_warr(dirs);
	if (!item)
		return (dstr_new(arg));
	return (item);
}

DSTRING			*get_cdpath(char *arg, ENV *env)
{
	DSTRING		*cdpath;
	DSTRING		*path;

	if (*arg == '/')
		return (dstr_new(arg));
	cdpath = env_get_variable("CDPATH", env);
	if (!cdpath)
		sys_fatal_memerr("ALLOCATIONS FAILS CDPATH");
	dstr_trim_this(&cdpath);
	if (cdpath->strlen)
		path = cdpath_get_path(cdpath, arg);
	else
		path = dstr_new(arg);
	dstr_del(&cdpath);
	return (path);
}

int				cd_parse_args(char **args, ENV *env, DSTRING **path)
{
	int			mode;
	int			i;

	mode = CD_MODE_LOGIC;
	i = 1;
	if (args[1] && args[1][0] && args[1][0] == '-' \
	&& (args[1][1] == 'P' || args[1][1] == 'L'))
	{
		i = 2;
		mode = (args[1][1] == 'P') ? CD_MODE_PHYSP : CD_MODE_LOGIC;
	}
	if (!args[i])
		*path = env_get_variable("HOME", env);
	else if (args[i] && args[i][0] == '-' && args[i][1] == '\0')
		*path = dstr_new(g_oldpwd->txt);
	else
		*path = get_cdpath(args[i], env);
	!(*path) ? sys_fatal_memerr("cd: ALLOCATION FAILED") : 0;
	mode = cd_check_path(mode, *path);
	return (mode);
}
