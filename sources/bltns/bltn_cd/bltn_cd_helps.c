/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_cd_helps.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 16:03:08 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/11 13:06:27 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

void		bltn_cd_freepaths(DSTRING **oldpath, DSTRING **newpath,\
								t_dlist **path)
{
	dstr_del(oldpath);
	dstr_del(newpath);
	bltn_cd_destroy_path(path);
}

int			bltn_cd_error(DSTRING **oldpath, DSTRING **newpath,\
								t_dlist **path)
{
	int	perms;

	perms = sys_touch_file((*newpath)->txt);
	bltn_cd_freepaths(oldpath, newpath, path);
	if (!(perms & SYS_TCH_F))
		sys_error_handler("cd", E_NOFND, 0);
	else if ((perms & SYS_TCH_TYP) != SYS_TCH_DIR)
		sys_error_handler("cd", E_NODIR, 0);
	else if (!(perms & SYS_TCH_X))
		sys_error_handler("cd", E_PERMF, 0);
	return (1);
}

void		bltn_cd_concat(char **path)
{
	char	*tmp;

	tmp = ft_strdup((*path));
	free((*path));
	(*path) = ft_strjoin("/", tmp);
	free(tmp);
}
