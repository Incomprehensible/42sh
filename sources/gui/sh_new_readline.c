/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_new_readline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 16:41:47 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/04 17:01:13 by hgranule         ###   ########.fr       */
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

int				new_reg_expr(DSTRING **buf, t_indch *indch)
{
	DSTRING		*reg;
	t_astr		rez;

	reg = cut_reg_expr(*buf);
	ft_bzero(&rez, sizeof(t_astr));
	loop(reg, 0, &rez, 0);
	dstr_del(&reg);
	if (rez.count > 0)
		fill_buf(buf, rez);
	free_darr_n(rez.strings, rez.count);
	indch->tab = 0;
	indch->reg = 0;
	indch->fl = 0;
	return ((*buf)->strlen);
}

DSTRING			*new_return_line(DSTRING **buf, t_indch indch)
{
	if (indch.ch == (char)0x04)
	{
		dstr_del(buf);
		free_lines_down();
		ft_putstr("\n");
		sys_term_restore();
		return (dstr_nerr("exit"));
	}
	free_lines_down();
	sh_new_rewrite(indch.prompt, *buf, indch.ind);
	ft_putstr("\n");
	if (indch.reg)
		new_reg_expr(buf, &indch);
	sys_term_restore();
	return (*buf);
}

DSTRING			*sh_new_redline(const DSTRING *prompt, ENV *env)
{
	DSTRING		*buf;
	t_indch		indch;

	buf = dstr_nerr("");
	prebuf = 0;
	preind = 0;
	sys_term_init();
	sys_term_noecho();
	ft_bzero(&indch, sizeof(t_indch));
	ft_putstr(prompt->txt);
	indch.prompt = (DSTRING *)prompt;
	while (1)
	{
		if (!indch.fl && (indch.ch != (char)0x04 && (indch.ch != '\n')))
			indch.ch = ft_getch();
		indch.fl = 0;
		if (indch.ch == (char)0x04 || (indch.ch == '\n') || indch.ch == -1)
			return (new_return_line(&buf, indch));
		if (ft_isprint(indch.ch) && indch.ch != ESC)
			dstr_insert_ch(buf, indch.ch, indch.ind++);
		if (is_reg(buf))
			indch.reg = 1;
		if (indch.ch == BAKSP || indch.ch == DEL)
			indch.ind = sh_del_char(&buf, indch.ind, indch.ch);
		else if (is_ctrl(indch))
			indch = management_line(indch, &buf, env);
		else if (indch.ch == TAB && indch.reg)
			indch.ind = new_reg_expr(&buf, &indch);
		else if (indch.ch == TAB && !indch.reg)
			indch = sh_tab(&buf, env, indch);
		else if (indch.ch == ESC)
			indch = sh_esc(indch, buf->strlen, &buf, env);
		sh_new_rewrite(indch.prompt, buf, indch.ind);
	}
	sys_term_restore();
	return (dstr_nerr(""));
}
