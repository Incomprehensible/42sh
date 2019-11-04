/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:52:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/18 12:43:08 by gdaemoni         ###   ########.fr       */
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

t_indch					management_line(t_indch indch, DSTRING **buf, ENV *envr)
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
		indch = sh_new_search_h(buf, envr, indch);
	return (indch);
}

static DSTRING			*return_line(DSTRING **buf, t_indch indch, t_fl fl)
{
	if (indch.ch == (char)0x04)
	{
		dstr_del(buf);
		free_lines_down();
		ft_putstr("\n");
		return (dstr_nerr("exit"));
	}
	free_lines_down();
	sh_rewrite(*buf, indch.ind);
	ft_putstr("\n");
	if (fl.reg)
		reg_expr(buf, &fl);
	return (*buf);
}

char				is_reg(DSTRING *buf)
{
	int		i;

	i = -1;
	while (buf->txt && buf->txt[++i])
	{
		if (ft_memchr("*?[", buf->txt[i], 3) && buf->txt[i + 1] != '\\')
			return (1);
		if (buf->txt[i + 1] == '\0')
			break ;
	}
	return (0);
}

// static t_indch			auto_correction(DSTRING **buf, t_indch indch, \
// 							t_fl *fl, ENV *env)
// {
// 	if (is_reg(*buf))
// 		fl->reg = 1;
// 	if ((indch.ch == BAKSP || indch.ch == DEL) && (fl->tab = 0) == 0)
// 		indch.ind = sh_del_char(buf, indch.ind, indch.ch);
// 	if (indch.ch == TAB && fl->reg)
// 		indch.ind = reg_expr(buf, fl);
// 	else if (indch.ch == TAB && fl->reg == 0 && fl->tab++ == 0)
// 		indch = sh_tab(buf, env, indch);
// 	if (ft_isprint(indch.ch) && fl->tab)
// 		dstr_insert_ch(*buf, indch.ch, indch.ind++);
// 	if (is_ctrl(indch))
// 		indch = management_line(indch, buf, env);
// 	if (((indch.ch == ESC) || indch.fl) && (fl->tab = 0) == 0)
// 		indch = sh_esc(indch, (*buf)->strlen, buf, env);
// 	return (indch);
// }

// DSTRING					*sh_readline(ENV *env)
// {
// 	DSTRING		*buf;
// 	t_indch		indch;
// 	t_fl		fl;

// 	buf = dstr_nerr("");
// 	ft_bzero(&indch, sizeof(t_indch));
// 	ft_bzero(&fl, sizeof(t_fl));
// 	ft_putstr(NAME);
// 	while (1 && !(fl.tab = 0))
// 	{
// 		if (!fl.tab && !indch.fl && (indch.ch != (char)0x04 && (indch.ch != '\n')))
// 			indch.ch = ft_getch();
// 		if (indch.ch == (char)0x04 || (indch.ch == '\n'))
// 			return (return_line(&buf, indch, fl));
// 		if (is_ctrl(indch))
// 			indch = management_line(indch, &buf, env);
// 		if (ft_isprint(indch.ch) && indch.ch != DEL \
// 			&& (!indch.fl || indch.fl == 2) && (fl.tab = 0) == 0)
// 			dstr_insert_ch(buf, indch.ch, indch.ind++);
// 		indch = auto_correction(&buf, indch, &fl, env);
// 		sh_rewrite(buf, indch.ind);
// 	}
// 	return (dstr_nerr(""));
// }
