/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 06:51:22 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/27 10:21:24 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

int			parse_brackets(const char *str, const char ch)
{
	int i;
	int fl;
	int otr;

	i = -1;
	fl = 0;
	otr = 0;
	while (str[++i] != ']' && str[i] != '\0')
	{
		if (str[i] == '!')
			otr = 1;
		if (str[i] == ch && (fl = 1))
			break ;
		if (str[i] == '-' && ch >= str[i - 1] && ch <= str[i + 1] && (fl = 1))
			break ;
	}
	if ((fl && !otr) || (!fl && otr))
	{	
		while (str[i] != ']')
			++i;
		return (i + 1);
	}
	return (-1);
}

int			nmatch(const char *s1, const char *s2)
{
	int		step;

	if ((*s1 != '\0') && (*s2 == '*'))
		return (nmatch(s1 + 1, s2) || nmatch(s1, s2 + 1));
	else if ((*s1 == *s2 && *s2 != '\0') || *s1 == '?' || *s2 == '?')
		return (nmatch(s1 + 1, s2 + 1));
	else if (*s1 == '[')
	{
		if ((step = parse_brackets(s1, *s2)) != -1)
			return (nmatch(s1 + step, s2 + 1));
		return (0);
	}
	else if (*s2 == '[')
	{
		if ((step = parse_brackets(s2, *s1)) != -1)
			return (nmatch(s1 + 1, s2 + step));
		return (0);
	}
	else if (*s1 == '\0' && *s2 == '\0')
		return (1);
	else if (*s1 == '\0' && *s2 == '*')
		return (nmatch(s1, s2 + 1));
	return (0);
}

DSTRING		*cut_reg_expr(DSTRING *buf)
{
	ssize_t		ind;

	ind = dstrrchr(buf, ' ');
	return (dstr_slice(buf, ind + 1, buf->strlen));
}

DSTRING			*slice_reg(DSTRING *reg)
{
	int		i;
	int		fl;
	DSTRING	*rez;

	i = -1;
	fl = 0;
	while (reg->txt[++i])
	{
		if (reg->txt[i] == '*' || reg->txt[i] == '?' || reg->txt[i] == '[')
			fl = 1;
		if (reg->txt[i] == '/' && fl)
		{
			rez = dstr_slice(reg, 0, i + 1);
			break ;
		}
		if (reg->strlen == i + 1 && fl)
			rez = dstr_new(reg->txt);
	}
	if (!rez)
		rez = dstr_new("");
	return (rez);
}

static void		fill_buf(DSTRING **buf, const t_astr rez)
{
	int		ind;
	int		j;

	ind = dstrrchr(*buf, ' ');
	if (ind == -1)
		ind = 0;
	j = -1;
	while (++j < rez.count)
		if (dstrrchr(rez.strings[j], '*') == -1)
			break ;
	dstr_cutins_str(buf, "", ind);
	while (j < rez.count)
	{
		if (ind != 0)
			dstr_insert_str((*buf), " ", (*buf)->strlen);
		dstr_insert_dstr((*buf), rez.strings[j++], (*buf)->strlen);
	}
}

int				astr(DSTRING **buf, t_fl *fl)
{
	DSTRING		*reg;
	t_astr		rez;
	t_indch		i;

	reg = cut_reg_expr(*buf);
	ft_bzero(&rez, sizeof(t_astr));
	loop(reg, 0, &rez, 0);
	dstr_del(&reg);
	if (rez.count > 0)
		fill_buf(buf, rez);
	free_darr_n(rez.strings, rez.count);
	fl->tab = 0;
	fl->astr = 0;
	return ((*buf)->strlen);
}
