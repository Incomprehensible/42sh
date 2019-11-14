/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:48:08 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/14 20:46:55 by gdaemoni         ###   ########.fr       */
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

DSTRING				*readlie_loop(DSTRING **buf, t_indch indch, ENV *env)
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
			indch = management_line(indch, buf, env);
		else if (indch.ch == TAB && indch.reg)
			new_reg_expr(buf, &indch);
		if (indch.ch == TAB)
			sh_new_tab(buf, &indch, env);
		else if (indch.ch == ESC)
			indch = sh_esc(indch, (*buf)->strlen, buf, env);
		sh_new_rewrite(indch.prompt, (*buf), indch.ind);
		// printf("--%d------------------>%d\n", indch.ind_inp, indch.type_inp);
	}
	return (dstr_nerr(""));
}

DSTRING				*sh_readline(const DSTRING *prompt, ENV *env)
{
	DSTRING			*buf;
	t_indch			indch;

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
