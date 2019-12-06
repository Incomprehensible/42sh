/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 20:49:27 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/06 22:58:46 by hgranule         ###   ########.fr       */
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
		*path = dstr_new(args[i]);
	!(*path) ? sys_fatal_memerr("cd: ALLOCATION FAILED") : 0;
	mode = cd_check_path(mode, *path);
	return (mode);
}
