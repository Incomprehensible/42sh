/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 10:36:36 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/15 09:01:08 by hgranule         ###   ########.fr       */
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

int			sys_write_history(DSTRING *line, ENV *envr)
{
	int			fd;
	char		*hist_path;

	if (line->strlen == 0 || ft_strequ(line->txt, "\n") || history_file_status-- > 0)
		return (0);
	history_file_status = history_file_status < 0 ? 0 : history_file_status;
	hist_path = sys_get_conf_path(SH_HIST_FILE, envr);
	dstr_insert_ch(line, '\n', line->strlen);
	if ((fd = sys_file_op(hist_path, envr, \
	O_WRONLY | O_APPEND, "Write_history error")) < 0)
	{
		history_file_status = 20;
		free((void *)hist_path);
		return (-1);
	}
	write(fd, line->txt, line->strlen);
	free((void *)hist_path);
	close(fd);
	return (0);
}
