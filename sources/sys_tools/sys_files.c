/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 10:36:36 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 07:45:06 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

int		f_edp(char *er_c, int ecode, ENV *envr, int ison)
{
	if (ison)
		return (sys_error_handler(er_c, ecode, envr));
	return (-ecode);
}

int		sys_file_op(const char *path, ENV *envr, \
const int flags, char *ercx)
{
	int				fd;
	const int		perms = sys_touch_file(path);
	const mode_t	file_mode = 0644;

	if (!(perms & SYS_TCH_F))
	{
		if ((fd = open(path, flags | O_CREAT, file_mode)) < 0)
			return (f_edp(ercx, E_FCRTF, envr, ercx != (char *)1 ? 1 : 0));
		return (fd);
	}
	if (!(perms & SYS_TCH_W) || !(perms & SYS_TCH_R))
		return (f_edp(ercx, E_PERMF, envr, ercx != (char *)1 ? 1 : 0));
	if (((perms & SYS_TCH_TYP) == SYS_TCH_DIR))
		return (f_edp(ercx, E_ISDIR, envr, ercx != (char *)1 ? 1 : 0));
	if ((fd = open(path, flags)) < 0)
		return (f_edp(ercx, E_FOPFF, envr, ercx != (char *)1 ? 1 : 0));
	return (fd);
}
