/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:52:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/27 12:18:30 by gdaemoni         ###   ########.fr       */
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
	if (fl.astr)
		astr(buf, &fl);
	return (*buf);
}

DSTRING			*sh_readline(t_envp *env)
{
	DSTRING		*buf;
	t_indch		indch;
	t_fl		fl;
	char		*move;

	buf = dstr_new(0);
	indch.ind = 0;
	ft_bzero(&fl, sizeof(t_fl));
	while (1)
	{
		if (fl.tab == 0)
			indch.ch = ft_getch();
		//  printf("0x%hhX == [%c]\n", indch.ch, (indch.ch < ' ' || indch.ch > '~') ? '$' : indch.ch);
		if (indch.ch == (char)0x04)
			return (control_c(&buf));
		else if (indch.ch == '\n')
			return (fill_reg(&buf, fl));
		else if ((indch.ch == BAKSP || indch.ch == DEL) && (fl.tab = 0) == 0)
			indch.ind = sh_del_char(&buf, indch.ind, indch.ch);
		else if (ft_isprint(indch.ch) && (fl.tab = 0) == 0)
			dstr_insert_ch(buf, indch.ch, indch.ind++);
		else if ((indch.ch == ESC) && (fl.tab = 0) == 0)
			indch.ind = sh_esc(indch.ind, buf->strlen);
		if (indch.ch == TAB && fl.astr == 0 && fl.tab++ == 0)
			indch = sh_tab(&buf, indch.ind, env);
		if (indch.ch == '*' || indch.ch == '?' || indch.ch == '[' || indch.ch == '!')
			fl.astr = 1;
		if (indch.ch == TAB && fl.astr)
			indch.ind = astr(&buf, &fl);
		sh_rewrite(buf, indch.ind);
	}
}
