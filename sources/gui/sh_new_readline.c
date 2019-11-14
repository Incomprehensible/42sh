/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_new_readline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 16:41:47 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/13 19:02:26 by gdaemoni         ###   ########.fr       */
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

void					new_reg_expr(DSTRING **buf, t_indch *indch)
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
	indch->ind = (*buf)->strlen;
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
	sh_new_rewrite(indch.prompt, *buf, indch.ind);
	ft_putstr("\n");
	if (indch.reg)
		new_reg_expr(buf, &indch);
	sys_term_restore();
	return ((*buf));
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

int				sh_del_char(DSTRING **buf, int ind, const char cmd)
{
	if (cmd == '~' && (*buf)->strlen > ind)
	{
		dstr_del_char(buf, (ssize_t)ind);
		return (ind);
	}
	if (cmd == BAKSP && (*buf)->strlen > 0)
	{
		dstr_del_char(buf, (ssize_t)--ind);
		return (ind);
	}
	return (ind);
}

int			get_ind_var(DSTRING *buf, t_indch *indch)
{
	int		i;

	i = indch->ind;
	indch->var = 0;
	while (--i > -1)
	{
		if (buf->txt[i] == '$')
		{
			indch->var = 1;
			return (i);
		}
		if (buf->txt[i] == ' ')
		{
			indch->var = 0;
			return (-1);
		}
	}
	return (-1);
}

void			insert_val(DSTRING **buf, DSTRING *val, int i_var)
{
	int			i;
	DSTRING		*tmp;

	i = i_var;
	while ((*buf)->txt[i] && (*buf)->txt[i] != ' ')
		++i;
	if (i == (*buf)->strlen)
		dstr_cutins_dstr(buf, val, i_var);
	else
	{
		tmp = dstr_slice((*buf), i, (*buf)->strlen);
		dstr_cutins_dstr(buf, val, i_var);
		dstr_insert_dstr((*buf), tmp, (*buf)->strlen);
		dstr_del(&tmp);
	}
}

t_indch			auto_cor_var(DSTRING **buf, t_indch indch, ENV *env, int i_var)
{
	DSTRING		*value;
	DSTRING		*variable;

	if (i_var == -1)
	{
		indch.var = 0;
		return (indch);
	}
	variable = dstr_slice((*buf), i_var, indch.ind);
	value = env_get_variable(variable->txt + 1, env);
	if (value->strlen > 0)
		insert_val(buf, value, i_var);
	dstr_del(&variable);
	dstr_del(&value);
	indch.is_var = 0;
	indch.var = 0;
	indch.fl_t = 1;
	indch.ind = (*buf)->strlen;
	return (indch);
}

DSTRING			*sh_readline_loop(DSTRING **buf, t_indch indch, \
					const DSTRING *prompt, ENV *env)
{
	while (1)
	{
		if (!indch.fl && (indch.ch != (char)0x04 && (indch.ch != '\n')))
			indch.ch = ft_getch();
		if (indch.ch == (char)0x04 || (indch.ch == '\n') || indch.ch == -1)
			return (new_return_line(buf, indch));
		if (ft_isprint(indch.ch) && indch.ch != ESC)
			dstr_insert_ch((*buf), indch.ch, indch.ind++);
		if (!(indch.fl = 0) &&is_reg((*buf)))
			indch.reg = 1;
		if (!(indch.fl_t = 0) && indch.ch == BAKSP)
			indch.ind = sh_del_char(buf, indch.ind, indch.ch);
		// indch.type_inp = fl_input((*buf), &indch);
		if (is_ctrl(indch.ch))
			indch = management_line(indch, buf, env);
		else if (indch.ch == TAB && indch.reg)
			new_reg_expr(buf, &indch);
		else if (indch.ch == TAB && indch.is_var)
			indch = auto_cor_var(buf, indch, env, get_ind_var((*buf), &indch));
		if (indch.ch == TAB && !indch.reg && !indch.var && !indch.fl_t)
			indch = sh_tab(buf, env, indch);
		else if (indch.ch == ESC)
			indch = sh_esc(indch, (*buf)->strlen, buf, env);
		sh_new_rewrite(indch.prompt, (*buf), indch.ind);
		printf("-------------------->%d\n", indch.type_inp);
	}
	return (dstr_nerr(""));
}

DSTRING			*sh_new_redline(const DSTRING *prompt, ENV *env)
{
	DSTRING		*buf;
	t_indch		indch;

	buf = dstr_nerr("");
	g_prebuf = 0;
	g_preind = 0;
	sys_term_init();
	sys_term_noecho();
	ft_bzero(&indch, sizeof(t_indch));
	ft_putstr(prompt->txt);
	indch.prompt = (DSTRING *)prompt;
	sh_readline_loop(&buf, indch, prompt, env);
	sys_term_restore();
	return (buf);
}
