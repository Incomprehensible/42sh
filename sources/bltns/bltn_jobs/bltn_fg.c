/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_fg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 07:43:43 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/13 21:50:00 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_hidden.h"
#include "libftprintf.h"
#include "bltn.h"

int				bltn_fg_bg_launch(ssize_t a, int mode)
{
	t_dlist		*pd_lst;
	t_ps_d		*psd;
	
	if (g_ptab[a] && g_ptab[a]->mode == PS_M_BG)
	{
		pd_lst = g_ptab[a]->members;
		while (pd_lst)
		{
			psd = (t_ps_d *)&pd_lst->size;
			if (psd->state == PS_S_STP)
				psd->state = PS_S_RUN;
			pd_lst = pd_lst->next;
		}
		g_ptab[a]->state = PS_S_RUN;
		g_ptab[a]->mode = mode;
		return (0);
	}
	return (1);
}

ssize_t		fg_by_str(char *job)
{
	ssize_t		i;

	i = g_jid;
	while (--i > 0)
	{
		if (g_ptab[i])
		{
			if (g_ptab[i]->input_line && ft_strstr(g_ptab[i]->input_line, job))
				return (i);
		}
	}
	return (0);
}

ssize_t			fg_args_get_jid(char *job)
{
	ssize_t		a;

	if (ft_str_isnumeric(job, 10))
	{
		if (1 > (a = ft_atoll_base(job, 10)))
			a = fg_by_str(job);
		if (!g_ptab[a])
			a = fg_by_str(job);
	}
	else
		a = fg_by_str(job);
	return (a);
}

int				fg_bg_continue(ssize_t a, int is_fg_mode)
{
	if (is_fg_mode)
		tcsetpgrp(0, g_ptab[a]->pgid);
	killpg(g_ptab[a]->pgid, SIGCONT);
	ft_printf("%s <<\n", g_ptab[a]->input_line);
	return (0);
}

int				bltn_fg(char **args, ENV *envr)
{
	ssize_t		a;
	ssize_t		j;

	if (!args[1])
	{
		a = g_jid;
		while (--a > 0)
			if (0 == bltn_fg_bg_launch(a, PS_M_FG))
				break ;
		if (a <= 0)
			return (sys_perror("fg: no jobs left!", 2, envr));
		fg_bg_continue(a, 1);
		return (0);
	}
	j = 0;
	while (args[++j])
	{
		if (1 > (a = fg_args_get_jid(args[j])))
			return (no_job_error(args[j], "fg: ", envr));
		bltn_fg_bg_launch(a, PS_M_FG);
		fg_bg_continue(a, 1);
	}
	return (1);
}

int				bltn_bg(char **args, ENV *envr)
{
	ssize_t		a;
	ssize_t		j;

	if (!args[1])
	{
		a = g_jid;
		while (--a > 0)
			if (0 == bltn_fg_bg_launch(a, PS_M_BG))
				break ;
		if (a <= 0)
			return (sys_perror("bg: no jobs left!", 2, envr));
		fg_bg_continue(a, 0);
		return (0);
	}
	j = 0;
	while (args[++j])
	{
		if (1 > (a = fg_args_get_jid(args[j])))
			return (no_job_error(args[j], "bg: ", envr));
		bltn_fg_bg_launch(a, PS_M_BG);
		fg_bg_continue(a, 0);
	}
	return (1);
}

