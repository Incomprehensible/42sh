/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:48:08 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/19 02:34:30 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "dstring.h"
#include <dirent.h>

t_indch			management_line(t_indch indch, DSTRING **buf)
{
	DSTRING		*str;

	if (indch.ch == 0x1)
		indch.ind = 0;
	else if (indch.ch == 0x5)
		indch.ind = (*buf)->strlen;
	else if (indch.ch == 0x15)
	{
		str = dstr_serr(*buf, indch.ind, (*buf)->strlen);
		dstr_del(buf);
		*buf = str;
		indch.ind = 0;
	}
	else if (indch.ch == 0x14)
		dstr_cutins_str(buf, "", indch.ind);
	else if ((indch.ch == 0x18 || indch.ch == 0x06) && (*buf)->strlen)
		indch.ind = sh_move_insertion_point((*buf), indch.ind, indch.ch);
	else if (indch.ch == 0x0e)
		clear_screen();
	else if (indch.ch == 0x12)
		indch = sh_search_his(buf, indch);
	return (indch);
}

DSTRING			*new_return_line(DSTRING **buf, t_indch indch)
{
	if (indch.ch == (char)0x04)
	{
		dstr_del(buf);
		free_lines_down();
		ft_putstr("\n");
		sys_term_restore();
		(*buf) = dstr_nerr("exit");
		return ((*buf));
	}
	free_lines_down();
	sh_rewrite(indch.prompt, *buf, indch.ind);
	ft_putstr("\n");
	if (indch.reg)
		new_reg_expr(buf, &indch);
	sys_term_restore();
	return ((*buf));
}

int				sh_del_char(DSTRING **buf, int ind, const char cmd)
{
	if (cmd == '~' && (*buf)->strlen > ind)
	{
		dstr_del_char(buf, (ssize_t)ind);
		return (ind);
	}
	if (cmd == BAKSP && (*buf)->strlen > 0 && ind > 0)
	{
		dstr_del_char(buf, (ssize_t)--ind);
		return (ind);
	}
	return (ind);
}

DSTRING			*readlie_loop(DSTRING **buf, t_indch indch, ENV *env)
{
	while (1)
	{
		if (!indch.fl && (indch.ch != (char)0x04 && (indch.ch != '\n')))
			indch.ch = ft_getch();
		if (!(indch.fl = 0) && indch.ch == BAKSP)
			indch.ind = sh_del_char(buf, indch.ind, indch.ch);
		if (indch.ch == (char)0x04 || (indch.ch == '\n') || indch.ch == -1)
			return (new_return_line(buf, indch));
		if (ft_isprint(indch.ch) && indch.ch != ESC)
			dstr_insert_ch((*buf), indch.ch, indch.ind++);
		sh_type_input((*buf), &indch);
		if (is_ctrl(indch.ch))
			indch = management_line(indch, buf);
		else if (indch.ch == TAB && indch.reg)
			new_reg_expr(buf, &indch);
		if (indch.ch == TAB)
			sh_tab(buf, &indch, env);
		else if (indch.ch == ESC)
			indch = sh_esc(indch, (*buf)->strlen, buf);
		sh_rewrite(indch.prompt, (*buf), indch.ind);
	}
	return (dstr_nerr(""));
}

DSTRING			*sh_readline(const DSTRING *prompt, ENV *env)
{
	DSTRING		*buf;
	t_indch		indch;

	buf = dstr_nerr("");
	ft_bzero(&indch, sizeof(t_indch));
	g_prebuf = 0;
	g_preind = 0;
	sys_term_init();
	sys_term_noecho();
	ft_putstr(prompt->txt);
	indch.prompt = (DSTRING *)prompt;
	readlie_loop(&buf, indch, env);
	sys_term_restore();
	return (buf);
}
