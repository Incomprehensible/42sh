/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_proc_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 11:07:06 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/06 23:36:24 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"

#include "stdio.h"

extern t_pgrp	*p_table[SYS_PRGS_SIZE];

int			sys_wait_ps(t_dlist *ps, int *status, pid_t lpid)
{
	t_ps_d	*psd;
	pid_t	pid;
	int		statl;
	int		opt;
	int		signal;

	opt = WUNTRACED;
	psd = (t_ps_d *)&(ps->size);
	if (psd->pid == 0)
		return (psd->state);
	if ((pid = waitpid(psd->pid, &statl, opt)) > 0)
	{
		if (WIFEXITED(statl))
		{
			//debug
			int		rstat = WEXITSTATUS(statl);

			if (pid == lpid)
				*status = rstat;
			//printf ("exit code %d\n", rstat);
			psd->state = PS_S_DON;
			psd->pid = 0;
		}
		else if (WIFSTOPPED(statl))
		{
			signal = WSTOPSIG(statl);
			psd->state = PS_S_STP;
		}
		else if (WIFSIGNALED(statl))
		{
			signal = WTERMSIG(statl);
			if (pid == lpid)
				*status = 128 + signal;
			psd->state = PS_S_SIG;
		}
		return (psd->state);
	}
	return (0);
}

int			sys_wait_prg(t_pgrp **ps_grp, int *status, pid_t lpid)
{
	int		state;
	t_dlist	*ps;
	t_ps_d	*psd;

	if ((*ps_grp)->state == PS_S_DON || (*ps_grp)->state == PS_S_SIG)
		return (sys_delete_prg(ps_grp));
	(*ps_grp)->state = PS_S_DON;
	ps = (*ps_grp)->members;
	while (ps)
	{
		psd = (t_ps_d *)&(ps->size);
		state = sys_wait_ps(ps, status, lpid);
		ps = ps->next;
		// RESETTING STATE OF WHOLE GROUP
		(*ps_grp)->state = (*ps_grp)->state < state ? state : (*ps_grp)->state;
	}
	return (0);
}

/*
** ТРебуется закончить и отследить конец всех процессов в группе
*/

int			sys_wait_ptable(int *status, pid_t lpid)
{
	ssize_t i;

	i = -1;
	while (++i < SYS_PRGS_SIZE)
	{
		if (p_table[i] != 0)
			sys_wait_prg(&p_table[i], status, lpid);
	}
	return (0);
}
