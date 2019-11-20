/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_jobs_entries.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 15:46:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 15:47:03 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "bltn.h"
#include "libftprintf.h"

int		jobs_put_entry_sm(const ssize_t i)
{
	t_pgrp		*pg;
	char		cur;
	char		*state_str;

	if (!(pg = g_ptab[i]))
		return (0);
	cur = jobs_cur_char_set(i);
	state_str = jobs_state_str_set(pg);
	ft_printf("[%zd]%c  %-26s %s\n", i, cur, state_str, pg->input_line);
	free(state_str);
	return (1);
}

int		jobs_put_pids(const ssize_t i)
{
	t_pgrp		*pg;

	if (!(pg = g_ptab[i]))
		return (0);
	ft_printf("%d\n", pg->pgid);
	return (1);
}

int		jobs_put_long_entry(const ssize_t i)
{
	t_pgrp		*pg;
	t_dlist		*pss;
	t_ps_d		*psd;
	char		cur;
	char		*state_str;

	if (!(pg = g_ptab[i]))
		return (0);
	cur = jobs_cur_char_set(i);
	state_str = jobs_state_str_set(pg);
	pss = pg->members;
	psd = (t_ps_d *)&pss->size;
	ft_printf("[%zd]%c %5d %-26s %s\n", i, cur, psd->pid, \
	state_str, pss->content);
	while ((pss = pss->next))
	{
		psd = (t_ps_d *)&pss->size;
		ft_printf("     %5d %26s %s\n", psd->pid, " ", pss->content);
	}
	free(state_str);
	return (1);
}

int		jobs_put_by_mode(const ssize_t i, int md)
{
	if (g_jid >= i)
	{
		if (md == 0)
			return (jobs_put_entry_sm(i));
		if (md == 1)
			return (jobs_put_pids(i));
		if (md == 2)
			return (jobs_put_long_entry(i));
		return (-1);
	}
	return (0);
}
