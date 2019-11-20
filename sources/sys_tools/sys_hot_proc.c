/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_hot_proc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 07:56:46 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 08:00:37 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"

int			sys_hot_charge(pid_t pid, int mode, char *str)
{
	extern pid_t	g_hgid;
	t_pgrp			*prg;
	char			*tmp;

	if (pid == 0)
		return (-1);
	if (g_hgid == 0)
	{
		g_hgid = pid;
		prg = sys_prg_create(pid, 0, 0, mode);
	}
	else
		prg = sys_prg_get(g_hgid);
	tmp = sys_get_prg_iline(prg->input_line, str);
	if (prg->input_line)
		free(prg->input_line);
	prg->input_line = tmp;
	sys_prc_create(pid, &(prg->members), str);
	return (0);
}

int			sys_hot_off(int ret_status)
{
	extern pid_t	g_hgid;
	extern char		*g_hbg;

	g_hgid = 0;
	g_hbg = 0;
	return (ret_status);
}
