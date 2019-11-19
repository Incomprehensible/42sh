/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_proc_tables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 09:13:06 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 04:06:02 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "stdio.h"
#include "libftprintf.h"
#include "dstring.h"
#include "rms.h"

extern t_pgrp	*g_ptab[SYS_PRGS_SIZE];

/*
** GRPOUP DESCRIPTOR содержит адресс на двусвязнный список
** который в свою очередь является последовательностью попроцессов в группе
** В каждом list->size лежит PID процесса и больше ничего.
*/

int			sys_prc_create(pid_t pid, t_dlist **members, char *str)
{
	t_dlist	*prc;
	t_ps_d	*psd;

	if (!members)
		return (-1);
	if (!(prc = ft_dlstnew_cc(0, 0)))
		sys_fatal_memerr("PROC_REC_FAILED");
	psd = (t_ps_d *)(&(prc->size));
	psd->pid = pid;
	psd->state = PS_S_RUN;
	psd->signal = 0;
	prc->content = str;
	psd->exit_st = (unsigned char)255;
	ft_dlstpush(members, prc);
	return (0);
}

t_pgrp		*sys_prg_create(pid_t prg, t_dlist *prcs, char *str, int mode)
{
	extern int	g_jid;

	if (!prg)
		return (0);
	if (!(g_ptab[g_jid] = ft_memalloc(sizeof(t_pgrp))))
		sys_fatal_memerr("PROC_GROUP_FAILED");
	g_ptab[g_jid]->pgid = prg;
	g_ptab[g_jid]->members = prcs;
	g_ptab[g_jid]->mode = mode;
	g_ptab[g_jid]->input_line = str;
	g_ptab[g_jid]->state = PS_S_RUN;
	g_ptab[g_jid]->signal = -1;
	g_ptab[g_jid]->flag = 0;
	if (mode == PS_M_BG)
		ft_printf("[%d] %d\n", g_jid, prg);
	++g_jid;
	return (g_ptab[g_jid - 1]);
}

t_pgrp		*sys_prg_get(pid_t prg)
{
	ssize_t	i;

	i = g_jid;
	while (--i > 0)
	{
		if (g_ptab[i] && g_ptab[i]->pgid == prg)
			return (g_ptab[i]);
	}
	return (0);
}

int			sys_delete_prg(t_pgrp **prg)
{
	if ((*prg)->members)
		ft_dlst_delf(&(*prg)->members, 0, et_rm_prc);
	if ((*prg)->input_line)
		free((*prg)->input_line);
	free(*prg);
	*prg = 0;
	return (0);
}

char		*sys_get_prg_iline(char *old_il, char *new_cm)
{
	char			*res;
	t_dyn_string	*buff;

	if (!new_cm)
		return (old_il);
	if (!(buff = dstr_new(new_cm)))
		sys_fatal_memerr("ERR_P_TABLE_COMM_FAIL");
	if (old_il)
	{
		dstr_insert_str(buff, " ", 0);
		dstr_insert_str(buff, old_il, 0);
	}
	if (!(res = ft_strdup(buff->txt)))
		sys_fatal_memerr("ERR_P_TABLE_COMM_FAIL");
	dstr_del(&buff);
	return (res);
}

int			sys_hot_charge(pid_t pid, int mode, char *str)
{
	extern pid_t	hot_gid;
	t_pgrp			*prg;
	char			*tmp;

	if (pid == 0)
		return (-1);
	if (hot_gid == 0)
	{
		hot_gid = pid;
		prg = sys_prg_create(pid, 0, 0, mode);
	}
	else
		prg = sys_prg_get(hot_gid);
	tmp = sys_get_prg_iline(prg->input_line, str);
	if (prg->input_line)
		free(prg->input_line);
	prg->input_line = tmp;
	sys_prc_create(pid, &(prg->members), str);
	return (0);
}

int			sys_hot_off(int ret_status)
{
	extern pid_t	hot_gid;
	extern char		*g_hbg;

	hot_gid = 0;
	g_hbg = 0;
	return (ret_status);
}
