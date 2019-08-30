/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:52:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 09:01:18 by gdaemoni         ###   ########.fr       */
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

// справочники
// http://www.codenet.ru/progr/cpp/7/5.php
// http://rus-linux.net/MyLDP/BOOKS/lpg-04/node8.htm

DSTRING			*control_c(DSTRING **buf)
{
	dstr_del(buf);
	return (dstr_new("exit()"));
}

DSTRING			*fill_reg(DSTRING **buf, t_fl fl)
{
	if (fl.reg)
		reg_expr(buf, &fl);
	return (*buf);
}

t_indch			management_line(t_indch indch, DSTRING **buf)
{
	if (indch.ch == 0x1)
		indch.ind = 0;
	else if (indch.ch == 0x5)
		indch.ind = (*buf)->strlen;
	return (indch);
}

DSTRING			*sh_readline(t_envp *env)
{
	DSTRING		*buf;
	t_indch		indch;
	t_fl		fl;
	char		*move;

	buf = dstr_new(0);
	ft_bzero(&indch, sizeof(t_indch));
	ft_bzero(&fl, sizeof(t_fl));
	while (1)
	{
		if (fl.tab == 0 && !indch.fl)
			indch.ch = ft_getch();
		printf("0x%hhX == [%c]\n", indch.ch, (indch.ch < ' ' || indch.ch > '~') ? '$' : indch.ch);
		if (indch.ch == (char)0x04)
			return (control_c(&buf));
		if (indch.ch == 0x1 || indch.ch == 0x5 || indch.ch == 0x15 || indch.ch == 0x14)
			indch = management_line(indch, &buf);
		if (indch.ch == '\n')
			return (fill_reg(&buf, fl));
		if ((indch.ch == BAKSP || indch.ch == DEL) && (fl.tab = 0) == 0)
			indch.ind = sh_del_char(&buf, indch.ind, indch.ch);
		if ((ft_isprint(indch.ch) || indch.ch == '\n') && (!indch.fl || indch.fl == 2) && (fl.tab = 0) == 0)
			dstr_insert_ch(buf, indch.ch, indch.ind++);
		if (indch.ch == TAB && fl.reg == 0 && fl.tab++ == 0)
			indch = sh_tab(&buf, indch.ind, env, indch);
		if (((indch.ch == ESC) || indch.fl) && (fl.tab = 0) == 0)
			indch = sh_esc(indch, buf->strlen, &buf);
		if (ft_memchr("*?[", indch.ch, 3))
			fl.reg = 1;
		if (indch.ch == TAB && fl.reg)
			indch.ind = reg_expr(&buf, &fl);
		free_lines_down();
		sh_rewrite(buf, indch.ind);
	}
}
