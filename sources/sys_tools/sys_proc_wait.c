/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_proc_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 11:07:06 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 08:06:38 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"
#include "bltns_jobs.h"

extern t_pgrp	*g_ptab[SYS_PRGS_SIZE];

void		sys_prg_sig_set(t_pgrp **ps_grp)
{
	t_dlist	*ps;
	t_ps_d	*psd;

	ps = (*ps_grp)->members;
	while (((*ps_grp)->state == PS_S_SIG || (*ps_grp)->state == PS_S_STP) \
	&& (psd = (t_ps_d *)&ps->size))
	{
		if (psd->signal > 0 && ((*ps_grp)->signal = psd->signal))
			break ;
		ps = ps->next;
	}
}

int			sys_wait_prg_jobs_checks(t_pgrp **ps_grp, int state, int o_state)
{
	if (o_state != state)
		(*ps_grp)->flag = 1;
	if ((*ps_grp)->state == PS_S_STP)
		(*ps_grp)->mode = PS_M_BG;
	if ((*ps_grp)->mode == PS_M_BG && \
	((*ps_grp)->state != PS_S_RUN) && (*ps_grp)->flag)
		return (42);
	if (!g_hsh && (*ps_grp)->mode == PS_M_FG && \
	((*ps_grp)->state == PS_S_SIG || (*ps_grp)->state == PS_S_DON))
		return (sys_delete_prg(ps_grp));
	return (0);
}

int			sys_del_if_done(ssize_t i, int state_code)
{
	if (!g_ptab[i])
		return (0);
	if (state_code == 42)
		g_ptab[i]->flag = 0;
	if (!g_hsh && (g_ptab[i]->state == PS_S_DON || \
	g_ptab[i]->state == PS_S_SIG))
		sys_delete_prg(&g_ptab[i]);
	return (1);
}

void		sys_wait_ptable_entry(ssize_t i, int *status, pid_t lpid)
{
	int		state_status;

	state_status = 0;
	if (g_ptab[i] != 0 && g_ptab[i]->mode != PS_M_BG)
		state_status = sys_wait_prg(&g_ptab[i], status, lpid, WUNTRACED);
	else if (g_ptab[i] != 0)
		state_status = sys_wait_prg(&g_ptab[i], status, lpid, WUNTRACED \
		| WNOHANG | WCONTINUED);
	if (state_status == 42)
	{
		g_ptab[i]->flag = 0;
		jobs_put_entry_sm(i);
		if (g_ptab[i]->state != PS_S_RUN && g_ptab[i]->state != PS_S_STP)
			sys_delete_prg(&g_ptab[i]);
	}
}

int			sys_wait_ptable(int *status, pid_t lpid)
{
	ssize_t i;

	i = g_jid;
	while (i > 0)
	{
		sys_wait_ptable_entry(i, status, lpid);
		--i;
	}
	i = g_jid;
	while (i > 0 && !g_ptab[i])
		--i;
	g_jid = i + 1;
	if (!g_hsh && !g_hbg)
		tcsetpgrp(0, getpid());
	return (0);
}
