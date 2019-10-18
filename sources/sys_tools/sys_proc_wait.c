/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_proc_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 11:07:06 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/18 21:36:49 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"

#include "stdio.h"

extern t_pgrp	*p_table[SYS_PRGS_SIZE];

void		sys_wtps_ext(t_ps_d *psd, int *status, pid_t lpid, int statloc)
{
	int		rstat;

	rstat = WEXITSTATUS(statloc);
	if (psd->pid == lpid)
		*status = rstat;
	psd->state = PS_S_DON;
	psd->signal = 0;
	psd->pid = 0;
}

void		sys_wtps_sig(t_ps_d *psd, int *status, pid_t lpid, int statloc)
{
	int		signal;

	signal = WTERMSIG(statloc);
	if (psd->pid == lpid)
		*status = 128 + signal;
	psd->state = PS_S_SIG;
	psd->signal = signal;
}

void		sys_wtps_stp(t_ps_d *psd, int statloc)
{
	int		signal;

	signal = WSTOPSIG(statloc);
	psd->state = PS_S_STP;
	psd->signal = signal;
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
	t_dlist	*ps;
	t_dlist	*delp;
	t_ps_d	*psd;

	if (!hot_sbsh && ((*ps_grp)->state == PS_S_DON || (*ps_grp)->state == PS_S_SIG))
		return (sys_delete_prg(ps_grp));
	(*ps_grp)->state = PS_S_DON;
	ps = (*ps_grp)->members;
	while (ps)
	{
		psd = (t_ps_d *)&(ps->size);
		state = sys_wait_ps(ps, status, lpid, mode);
		ps = ps->next;
		// RESETTING STATE OF WHOLE GROUP
		(*ps_grp)->state = (*ps_grp)->state < state ? state : (*ps_grp)->state;
	}
	ps = (*ps_grp)->members;
	while (((*ps_grp)->state == PS_S_SIG || (*ps_grp)->state == PS_S_STP) \
	&& (psd = (t_ps_d *)&ps->size))
	{
		if (psd->signal > 0 && ((*ps_grp)->signal = psd->signal))
			break ;
		ps = ps->next;
	}
	if ((*ps_grp)->state == PS_S_STP)
		(*ps_grp)->mode = PS_M_BG;
	return (0);
}

int			sys_wait_ptable(int *status, pid_t lpid)
{
	ssize_t i;

	i = g_jid;
	while (i > 0)
	{
		if (p_table[i] != 0 && p_table[i]->mode != PS_M_BG)
			sys_wait_prg(&p_table[i], status, lpid, WUNTRACED);
		else if (p_table[i] != 0)
			sys_wait_prg(&p_table[i], status, lpid, WNOHANG | WCONTINUED);
		--i;
	}
	i = g_jid;
	while (i > 0 && !p_table[i])
		--i;
	g_jid = i + 1;
	if (!hot_sbsh)
		tcsetpgrp(0, getpid());
	return (0);
}
