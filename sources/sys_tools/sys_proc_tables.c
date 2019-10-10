/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_proc_tables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/02 09:13:06 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/10 13:52:44 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "stdio.h"

extern t_pgrp	*p_table[SYS_PRGS_SIZE];

/*
** GRPOUP DESCRIPTOR содержит адресс на двусвязнный список
** который в свою очередь является последовательностью попроцессов в группе
** В каждом list->size лежит PID процесса и больше ничего.
*/

int			sys_prc_create(pid_t pid, t_dlist **members)
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
	ft_dlstpush(members, prc);
	return (0);
}

t_pgrp		*sys_prg_create(pid_t prg, t_dlist *prcs, char *str, int mode)
{
	ssize_t	i;

	i = 0;
	if (!prg)
		return (0);
	while (p_table[i] != 0)
		++i;
	if (!(p_table[i] = ft_memalloc(sizeof(t_pgrp))))
		sys_fatal_memerr("PROC_GROUP_FAILED");
	p_table[i]->pgid = prg;
	p_table[i]->members = prcs;
	p_table[i]->mode = mode;
	p_table[i]->input_line = str;
	p_table[i]->state = PS_S_RUN;
	return (p_table[i]);
}

t_pgrp		*sys_prg_get(pid_t prg)
{
	ssize_t	i;

	i = 0;
	while (i < SYS_PRGS_SIZE)
	{
		if (p_table[i] && p_table[i]->pgid == prg)
			return (p_table[i]);
		++i;
	}
	return (0);
}

int			sys_delete_prg(t_pgrp **prg)
{
	if ((*prg)->members)
		ft_dlst_delf(&(*prg)->members, 0, 0);
	if ((*prg)->input_line)
		free((*prg)->input_line);
	free(*prg);
	*prg = 0;
	return (0);
}

int			sys_hot_charge(pid_t pid, int mode, char *str)
{
	extern pid_t	hot_gid;
	t_pgrp			*prg;
	t_dlist			*prc;
	ssize_t			i;

	i = 0;
	if (pid == 0)
		return (-1);
	if (hot_gid == 0)
	{
		hot_gid = pid;
		prg = sys_prg_create(pid, 0, str, mode);
	}
	else
		prg = sys_prg_get(hot_gid);
	sys_prc_create(pid, &(prg->members));
	return (0);
}

int			sys_hot_off(int ret_status)
{
	extern pid_t	hot_gid;

	hot_gid = 0;
	return (ret_status);
}

// DEBUG
int			DBG_SYS_SNAP(void)
{
	ssize_t		i;
	t_pgrp		*pg;
	t_ps_d		*psd;
	char		*states[] = {"NULL", "DONE", "SIGNALED", "STOPPED", "RUNNING"};
	char		*mode[] = {"NULL", "FG", "BG"};
	t_dlist		*pids;

	i = -1;
	while (++i < SYS_PRGS_SIZE)
	{
		if (!p_table[i])
			continue;
		pg = p_table[i];
		printf(
			"\033[31mPRG_DBG: %d / %p\033[0m\n"
			"\tPROC GROUP No_ %ld\n"
			"\tPGID: %d\n"
			"\tSTATE: %s\n"
			"\tMODE: %s\n"
			"\tPIDS: "
			, hot_sbsh, &hot_sbsh, i, pg->pgid, states[pg->state], mode[pg->mode]);
		pids = pg->members;
		while (pids)
		{
			psd = (t_ps_d *)&pids->size;
			printf("%d:%c%c%c ", psd->pid, *(states[psd->state]), *(states[psd->state] + 1), \
			*(states[psd->state] + 2));
			pids = pids->next;
		}
		printf("\n\n");
	}
	return (0);
}

int			DBG_SYS_GLB(void)
{
	printf(
		"\033[31mGL_DBG: %d / %p\033[0m\n"
		"\tPPID: %d\n"
		"\tPID: %d\n"
		"\tADR: &sys_pipes=%p / &hot_gid=%p\n\n"
		, hot_sbsh, &hot_sbsh, getppid(), getpid(), &sys_pipes, &hot_gid);
	return (0);
}
