/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_touch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 22:05:35 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/22 08:41:23 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"
#include <sys/stat.h>

int		sys_is_ex_bin(const char *path)
{
	int	perms;

	perms = sys_touch_file(path);
	if (!(perms & SYS_TCH_F))
		return (-1);
	if ((perms & SYS_TCH_TYP) == SYS_TCH_DIR)
		return (-2);
	if (!(perms & SYS_TCH_X))
		return (-3);
	if (((perms & SYS_TCH_TYP) == SYS_TCH_REG) && (perms & SYS_TCH_X))
		return (0);
	return (-4);
}

int		sys_touch_file(const char *path)
{
	int				perms;
	struct stat		st;

	perms = 0;
	if (access(path, F_OK) == 0)
		perms |= 8;
	if (access(path, X_OK) == 0)
		perms |= 1;
	if (access(path, W_OK) == 0)
		perms |= 2;
	if (access(path, R_OK) == 0)
		perms |= 4;
	if (!stat(path, &st))
		perms |= ((st.st_mode & 00170000) >> 8);
	return (perms);
}
