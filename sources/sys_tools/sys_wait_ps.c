/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_wait_ps.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 08:03:51 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 08:06:46 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"
#include "bltns_jobs.h"

extern t_pgrp	*g_ptab[SYS_PRGS_SIZE];

void		sys_wtps_ext(t_ps_d *psd, int *status, pid_t lpid, int statloc)
{
	int		rstat;

	rstat = WEXITSTATUS(statloc);
	if (psd->pid == lpid)
		*status = rstat;
	psd->state = PS_S_DON;
	psd->signal = (char)0;
	psd->exit_st = (unsigned char)rstat;
}

void		sys_wtps_sig(t_ps_d *psd, int *status, pid_t lpid, int statloc)
{
	int		signal;
	int		rstat;

	signal = WTERMSIG(statloc);
	rstat = 128 + signal;
	if (psd->pid == lpid)
		*status = rstat;
	psd->state = PS_S_SIG;
	psd->signal = (char)signal;
	psd->exit_st = (unsigned char)rstat;
}

void		sys_wtps_stp(t_ps_d *psd, int statloc)
{
	int		signal;

	signal = WSTOPSIG(statloc);
	psd->state = PS_S_STP;
	psd->signal = (char)signal;
}

int			sys_wait_ps(t_dlist *ps, int *status, pid_t lpid, int mode)
{
	t_ps_d	*psd;
	pid_t	pid;
	int		statl;

	psd = (t_ps_d *)&(ps->size);
	if (psd->pid == 0)
		return (psd->state);
	if ((pid = waitpid(psd->pid, &statl, mode)) > 0)
	{
		if (WIFEXITED(statl))
			sys_wtps_ext(psd, status, lpid, statl);
		else if (WIFSTOPPED(statl))
			sys_wtps_stp(psd, statl);
		else if (WIFSIGNALED(statl))
			sys_wtps_sig(psd, status, lpid, statl);
		else if (WIFCONTINUED(statl))
			psd->state = PS_S_RUN;
		return (psd->state);
	}
	else if (pid == 0)
		return (psd->state);
	return (0);
}

int			sys_wait_prg(t_pgrp **ps_grp, int *status, pid_t lpid, int mode)
{
	int		state;
	int		o_state;
	t_dlist	*ps;
	t_ps_d	*psd;

	o_state = (*ps_grp)->state;
	(*ps_grp)->state = PS_S_DON;
	ps = (*ps_grp)->members;
	while (ps)
	{
		psd = (t_ps_d *)&(ps->size);
		state = sys_wait_ps(ps, status, lpid, mode);
		ps = ps->next;
		(*ps_grp)->state = (*ps_grp)->state < state ? state : (*ps_grp)->state;
	}
	sys_prg_sig_set(ps_grp);
	return (sys_wait_prg_jobs_checks(ps_grp, state, o_state));
}
