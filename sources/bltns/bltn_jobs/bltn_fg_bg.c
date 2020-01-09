/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_fg_bg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 07:43:43 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/20 15:54:19 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_hidden.h"
#include "libftprintf.h"
#include "bltn.h"

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
