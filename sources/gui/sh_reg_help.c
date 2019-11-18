/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_reg_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 21:42:47 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/18 22:05:23 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

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
			rez = dstr_serr(reg, 0, i + 1);
			break ;
		}
		if (reg->strlen == i + 1 && fl)
			rez = dstr_nerr(reg->txt);
	}
	if (!rez)
		rez = dstr_nerr("");
	return (rez);
}

DSTRING			*cut_reg_expr(DSTRING *buf)
{
	ssize_t		ind;

	ind = dstrrchr(buf, ' ');
	return (dstr_serr(buf, ind + 1, buf->strlen));
}

void			fill_buf(DSTRING **buf, const t_astr rez)
{
	int		ind;
	size_t	j;

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
		if (ind != 0 || rez.count > 1)
			dstr_insert_str((*buf), " ", (*buf)->strlen);
		dstr_insert_dstr((*buf), rez.strings[j++], (*buf)->strlen);
	}
}

t_regpath		get_regpath(DSTRING *reg)
{
	int			i;
	int			fl;
	int			fl2;
	DSTRING		*path;

	i = -1;
	fl = 1;
	fl2 = 1;
	path = NULL;
	while (++i < reg->strlen)
	{
		if (reg->txt[i] == '*' || reg->txt[i] == '?' || reg->txt[i] == '[')
			fl = 0;
		if (reg->txt[i] == '/' && fl)
		{
			if (path)
				dstr_del(&path);
			path = dstr_serr(reg, 0, i + 1);
			fl2 = 2;
		}
	}
	return (help_get_regpath(fl2, path));
}
