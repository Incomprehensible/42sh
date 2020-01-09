/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_fg_bg_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 15:43:25 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 15:43:41 by hgranule         ###   ########.fr       */
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

ssize_t			fg_by_str(char *job)
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
