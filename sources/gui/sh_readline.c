/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:48:08 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/28 20:45:29 by hgranule         ###   ########.fr       */
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

void			clip_index(int x1, int x2, int *y1, int *y2)
{
	if (x1 > x2)
	{
		*y1 = x2;
		*y2 = x1;
	}
	else
	{
		*y2 = x2;
		*y1 = x1;
	}
}

void			clip_paste(t_indch *indch, t_clipbrd *clip, DSTRING **buf)
{
	indch->select = 0;
	if (clip->buffer->strlen)
	{
		dstr_insert_dstr(*buf, clip->buffer, indch->ind);
		indch->ind += clip->buffer->strlen;
	}
}

void			clip_cut(t_indch *indch, t_clipbrd *clip, DSTRING **buf)
{
	int			ei;
	int			bi;

	clip_index(clip->ind, indch->ind, &bi, &ei);
	if (clip->buffer)
		dstr_del(&(clip->buffer));
	indch->ind = bi;
	clip->buffer = dstr_scerr(buf, (ssize_t)bi, (ssize_t)ei);
	indch->select = 0;
}

void			clip_copy(t_indch *indch, t_clipbrd *clip, DSTRING **buf)
{
	int			ei;
	int			bi;

	clip_index(clip->ind, indch->ind, &bi, &ei);
	if (clip->buffer)
		dstr_del(&(clip->buffer));
	indch->ind = bi;
	clip->buffer = dstr_serr(*buf, (ssize_t)bi, (ssize_t)ei);
}

t_indch			management_line(t_indch indch, DSTRING **buf, t_clipbrd *clip)
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
	else if ((indch.ch == 0x02 || indch.ch == 0x06) && (*buf)->strlen)
		indch.ind = sh_move_insertion_point((*buf), indch.ind, indch.ch);
	else if (indch.ch == 0x0c)
		clear_screen();
	else if (indch.ch == 0x12)
		indch = sh_search_his(buf, indch);
	else if (indch.ch == 0xb && indch.select)
		clip_copy(&indch, clip, buf);
	else if (indch.ch == 0x10)
		clip_paste(&indch, clip, buf);
	else if (indch.ch == 0x18 && indch.select)
		clip_cut(&indch, clip, buf);
	return (indch);
}

void			return_line(DSTRING **buf, t_indch *indch, ENV *env)
{
	int			i;

	free_lines_down();
	if (indch->ch == (char)0x04 && !(*buf)->strlen)
	{
		indch->exit = 1;
		sys_term_restore();
		dstr_insert_str(*buf, "exit", 0);
		indch->exit = 1;
		return ;
	}
	else if (indch->ch == '\n')
	{
		indch->exit = 1;
		ft_putstr_fd("\n", STDOUT_FILENO);
		while ((i = is_reg(*buf)) != -1)
		{
			indch->ind_inp = i + 1;
			reg_expr(buf, indch, env);
		}
	}
	indch->fl = 0;
	indch->ch = 0;
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

DSTRING			*readline_loop(DSTRING **buf, t_indch indch, ENV *env, t_clipbrd *clip)
{
	while (!indch.exit)
	{
		if (!indch.fl && (indch.ch != (char)0x04 && (indch.ch != '\n')))
			indch.ch = ft_getch();
		if (!(indch.fl = 0) && indch.ch == BAKSP && !(indch.select = 0))
			indch.ind = sh_del_char(buf, indch.ind, indch.ch);
		else if (ft_isprint(indch.ch) && indch.ch != ESC && !(indch.select = 0))
			dstr_insert_ch((*buf), indch.ch, indch.ind++);
		sh_type_input((*buf), &indch);
		if (is_ctrl(indch.ch))
			indch = management_line(indch, buf, clip);
		else if (indch.ch == TAB && indch.reg && !indch.select)
			reg_expr(buf, &indch, env);
		else if (indch.ch == TAB && !indch.select)
			sh_tab(buf, &indch, env);
		else if (indch.ch == ESC)
			indch = sh_esc(indch, (*buf)->strlen, buf, clip);
		sh_rewrite(indch.prompt, (*buf), indch.ind, indch.select ? clip->ind : -1);
		if (indch.ch == (char)0x04 || (indch.ch == '\n') || indch.ch == -1)
			return_line(buf, &indch, env);
	}
	return (*buf);
}

DSTRING			*sh_readline(const DSTRING *prompt, ENV *env)
{
	DSTRING		*buf;
	t_indch		indch;
	t_clipbrd	clip;

	buf = dstr_nerr("");
	ft_bzero(&indch, sizeof(t_indch));
	clip.buffer = dstr_nerr("");
	g_prebuf = 0;
	g_preind = 0;
	sys_term_init();
	sys_term_noecho();
	ft_putstr_fd(prompt->txt, STDOUT_FILENO);
	indch.prompt = (DSTRING *)prompt;
	buf = readline_loop(&buf, indch, env, &clip);
	dstr_del(&clip.buffer);
	sys_term_restore();
	return (buf);
}
