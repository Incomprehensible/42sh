/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_jobs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 07:56:57 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 13:22:59 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "bltn.h"
#include "libftprintf.h"
#include "stdio.h"

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
	if (g_jid - 1 == i)
		return ('+');
	else if (g_jid - 2 == i && i > 0)
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

int		jobs_put_entry_sm(const ssize_t i)
{
	t_pgrp		*pg;
	char		cur;
	char		*state_str;

	if (g_jid >= i)
	{
		if (!(pg = g_ptab[i]))
			return (0);
		cur = jobs_cur_char_set(i);
		state_str = jobs_state_str_set(pg);
		ft_printf("[%zd]%c  %-26s %s\n", i, cur, state_str, pg->input_line);
		free(state_str);
		return (1);
	}
	return (0);
}

int		jobs_check_str_job(char *job_s)
{
	ssize_t		i;
	int			fl;
	int			state;
	int			ret;

	i = g_jid;
	fl = 0;
	while (--i > 0)
	{
		if (g_ptab[i])
		{
			if (g_ptab[i]->input_line && \
			ft_strstr(g_ptab[i]->input_line, job_s))
			{
				fl = 1;
				state = sys_wait_prg(&g_ptab[i], &ret, 0, \
				WUNTRACED | WNOHANG | WCONTINUED);
				jobs_put_entry_sm(i);
				sys_del_if_done(i, state);
			}
		}
	}
	if (fl)
		return (0);
	return (1);
}

int		jobs_check_num_job(char *num)
{
	ssize_t		i;
	int			ret;
	int			state;

	ret = 0;
	if (1 > (i = ft_atoll_base(num, 10)) || i >= g_jid)
		return (jobs_check_str_job(num));
	state = sys_wait_prg(&g_ptab[i], &ret, 0, WUNTRACED | WNOHANG | WCONTINUED);
	if (!jobs_put_entry_sm(i))
		return (jobs_check_str_job(num));
	sys_del_if_done(i, state);
	return (0);
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

int		bltn_jobs_iterate(char **args, ENV *envr)
{
	size_t	j;
	int		ret;

	j = 1;
	while (args[j])
	{
		if (ft_str_isnumeric(args[j], 10))
			ret = jobs_check_num_job(args[j]);
		else
			ret = jobs_check_str_job(args[j]);
		if (ret)
			no_job_error(args[j], "jobs: ", envr);
		++j;
	}
	return (ret);
}

int		bltn_jobs(char **args, ENV *envr)
{
	ssize_t		i;
	int			state;
	int			ret;

	i = g_jid;
	if (args_len(args) < 2)
		while (--i)
		{
			if (!g_ptab[i])
				continue ;
			state = sys_wait_prg(&g_ptab[i], &ret, 0, \
			WUNTRACED | WNOHANG | WCONTINUED);
			jobs_put_entry_sm(i);
			sys_del_if_done(i, state);
		}
	else
		bltn_jobs_iterate(args, envr);
	return (0);
}
