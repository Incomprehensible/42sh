/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_search_his.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 11:29:17 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/18 20:21:33 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

void		skip_escp(void)
{
	ft_getch();
	ft_getch();
}

void		print_his_line(DSTRING *str_srch, DSTRING *str_over)
{
	DSTRING			*prompt;
	DSTRING			*tmp;

	prompt = dstr_nerr("search in history ");
	if (!str_srch->strlen)
		sh_rewrite(prompt, str_srch, 0);
	else
	{
		tmp = dstr_nerr(str_srch->txt);
		dstr_insert_ch(tmp, '|', 0);
		dstr_insert_str(tmp, "| ", tmp->strlen);
		if (str_over != NULL)
			dstr_insert_str(tmp, str_over->txt, tmp->strlen);
		else
			dstr_insert_str(tmp, "", tmp->strlen);
		sh_rewrite(prompt, tmp, str_srch->strlen);
		dstr_del(&tmp);
	}
	dstr_del(&prompt);
}

void		bakspace_dstr(DSTRING **str_srch)
{
	if ((*str_srch)->strlen)
	{
		(*str_srch)->strlen--;
		(*str_srch)->txt[(*str_srch)->strlen] = '\0';
	}
}

void		print_his_help_col(t_darr overlap)
{
	ushort		col;
	int			iter;
	DSTRING		*colstr;

	iter = 0;
	col = get_col(overlap.maxlen + 2);
	ft_putstr("\n");
	ft_putstr(MOVEBGN);
	free_lines_down();
	colstr = sh_get_col(overlap, col, iter);
	ft_putstr(colstr->txt);
	ft_putstr("\n");
	dstr_del(&colstr);
}

void		print_his_col(DSTRING *str_srch)
{
	int					i;
	int					c;
	int					j;
	t_darr				rez;
	struct winsize		te;

	i = S_DARR_STRINGS;
	c = -1;
	j = 0;
	ft_bzero(&rez, sizeof(t_darr));
	ioctl(0, TIOCGWINSZ, &te);
	while (++c < g_histr.count)
	{
		if (ft_strncmp(str_srch->txt, g_histr.strings[--i]->txt,\
		str_srch->strlen) == 0 && g_histr.strings[i]->strlen < (te.ws_col - 3))
		{
			rez.strings[j++] = g_histr.strings[i];
			if (rez.maxlen < g_histr.strings[i]->strlen)
				rez.maxlen = g_histr.strings[i]->strlen;
			rez.allsize += g_histr.strings[i]->strlen;
		}
	}
	rez.count = j;
	if (j)
		print_his_help_col(rez.count ? rez : g_histr);
}

void		get_overlap_histr(t_darr *overlap, DSTRING *str_srch)
{
	int		i;
	int		c;
	int		j;

	c = -1;
	i = S_DARR_STRINGS;
	j = -1;
	ft_bzero(overlap, sizeof(t_darr));
	while (++c < g_histr.count)
	{
		if (g_histr.strings[--i]->txt && ft_strncmp(str_srch->txt, \
							g_histr.strings[i]->txt, str_srch->strlen) == 0)
		{
			overlap->strings[++j] = g_histr.strings[i];
			if (overlap->maxlen < g_histr.strings[i]->strlen)
				overlap->maxlen = g_histr.strings[i]->strlen;
			overlap->allsize += g_histr.strings[i]->strlen;
			overlap->count++;
		}
	}
}

t_indch		supplement_srch(DSTRING *str_srch, DSTRING **str_over, \
							t_indch indch)
{
	t_darr			overlap;
	int				i;

	i = 0;
	if ((*str_over) != NULL)
		ft_bzero((*str_over), sizeof(DSTRING));
	if (indch.ch == TAB)
		get_overlap_histr(&overlap, str_srch);
	while (indch.ch == TAB && overlap.count)
	{
		if (++i == overlap.count)
			i = 0;
		(*str_over) = overlap.strings[i];
		print_his_line(str_srch, (*str_over));
		indch.ch = ft_getch();
	}
	if (indch.ch == ESC)
		skip_escp();
	return (indch);
}

t_indch		control_histr(DSTRING **str_srch, DSTRING **str_over, t_indch indch)
{
	indch.ch = ft_getch();
	if (ft_isalpha(indch.ch) || indch.ch == ' ' || ft_isalnum(indch.ch))
		dstr_insert_ch((*str_srch), indch.ch, (*str_srch)->strlen);
	else if (indch.ch == BAKSP)
		bakspace_dstr(str_srch);
	if (indch.ch == ESC)
		skip_escp();
	if ((indch.ch != ' ' && !ft_isalpha(indch.ch) && indch.ch != 11 && \
		indch.ch != 12) && indch.ch != TAB && indch.ch != BAKSP && \
		!(indch.fl = 0) && !ft_isalnum(indch.ch))
		indch.ch = 0;
	indch = supplement_srch((*str_srch), str_over, indch);
	return (indch);
}

t_indch		sh_search_his(DSTRING **buf, ENV *envr, t_indch indch)
{
	DSTRING			*str_srch;
	DSTRING			*str_over;

	str_srch = dstr_nerr("");
	str_over = NULL;
	indch.ch = 1;
	indch.fl = 0;
	while (indch.ch && indch.ch != 4)
	{
		print_his_line(str_srch, str_over);
		indch = control_histr(&str_srch, &str_over, indch);
		if ((indch.ch != ' ' && !ft_isalpha(indch.ch) && indch.ch != 11 && \
			indch.ch != 12) && indch.ch != TAB && indch.ch != BAKSP && \
			!(indch.fl = 0) && !ft_isalnum(indch.ch))
			indch.ch = 0;
	}
	if (str_over && str_over->strlen)
	{
		dstr_del(buf);
		(*buf) = dstr_nerr(str_over->txt);
	}
	dstr_del(&str_srch);
	indch.ind = (*buf)->strlen;
	return (indch);
}
