/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_histr_help.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 21:50:07 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/18 22:07:23 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include "sys_tools/sys_tools.h"
#include <fcntl.h>
#include <unistd.h>

int				get_history_fd(int flags, char *er_context, ENV *envr)
{
	int			fd;
	char		*hist_pt;

	hist_pt = sys_get_conf_path(SH_HIST_FILE, envr);
	if ((fd = sys_file_op(hist_pt, envr, flags, er_context)) < 0)
	{
		free(hist_pt);
		return (-1);
	}
	free(hist_pt);
	return (fd);
}

void			init_histr(ENV *envr)
{
	ft_bzero(&g_histr, sizeof(t_darr));
	if (get_new_histr(&g_histr, envr) == 0)
		return ;
}
