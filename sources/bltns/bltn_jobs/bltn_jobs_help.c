/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_jobs_help.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 15:49:20 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 16:33:31 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "bltn.h"
#include "libftprintf.h"

char	*g_states[] = {
	"ERRMAC",
	"Done",
	"Terminated",
	"Suspended",
	"Running"
};

char	*g_signals[] = {
	"ERRMAC",
	"S_HUP (1)",
	"S_INT (2)",
	"S_QUIT (3)",
	"S_ILL (4)",
	"S_TRAP (5)",
	"S_ABRT (6)",
	"S_EMT (7)",
	"S_FPE (8)",
	"S_KILL (9)",
	"S_BUS (10)",
	"S_SEGV (11)",
	"S_SYS (12)",
	"S_PIPE (13)",
	"S_ALRM (14)",
	"S_TERM (15)",
	"S_URG (16)",
	"S_STOP (17)",
	"S_TSTP (18)",
	"S_CONT (19)",
	"S_CHLD (20)",
	"S_TTIN (21)",
	"S_TTOU (22)",
	"S_IO (23)",
	"S_XCPU (24)",
	"S_XFSZ (25)",
	"S_VTALRM (26)",
	"S_PROF (27)",
	"S_WINCH (28)",
	"S_INFO (29)",
	"S_USR1 (30)",
	"S_USR2 (31)"
};

char	jobs_cur_char_set(ssize_t i)
{
	ssize_t		j;

	j = g_jid - 1;
	while (g_ptab[j] == 0)
		--j;
	if (j == i)
		return ('+');
	while (g_ptab[j] == 0)
		--j;
	if (j == i)
		return ('-');
	return (' ');
}

char	*jobs_state_str_set(const t_pgrp *pg)
{
	t_dyn_string	*buff;
	t_ps_d			*psd;
	t_dlist			*member;
	char			*ex_str;

	if (!(buff = dstr_new(g_states[pg->state])))
		sys_fatal_memerr("JOBS_STATE_ERR_DSTR");
	if (pg->state == PS_S_DON)
	{
		if ((member = pg->members) && member->next == NULL)
		{
			psd = (t_ps_d *)&member->size;
			ex_str = ft_itoa((int)psd->exit_st);
			dstr_insert_str(buff, ": ", buff->strlen);
			dstr_insert_str(buff, ex_str, buff->strlen);
			free(ex_str);
		}
	}
	else if (pg->state == PS_S_SIG)
	{
		dstr_insert_str(buff, ": ", buff->strlen);
		dstr_insert_str(buff, g_signals[pg->signal], buff->strlen);
	}
	return (dstr_flush(&buff));
}

int		no_job_error(char *job, char *cont, ENV *envr)
{
	t_dyn_string	*buff;
	char			*err_str;

	if (!(buff = dstr_new("")))
		sys_fatal_memerr("JOBS_ERR_MSG_DSTR");
	dstr_insert_str(buff, cont, buff->strlen);
	dstr_insert_str(buff, job, buff->strlen);
	dstr_insert_str(buff, ": no such job", buff->strlen);
	if (!(err_str = dstr_flush(&buff)))
		sys_fatal_memerr("JOBS_ERR_MSG_DSTR");
	sys_perror(err_str, 1, envr);
	return (1);
}
