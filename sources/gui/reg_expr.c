/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reg_expr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 06:51:22 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/09/22 17:59:13 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

DSTRING			*cut_reg_expr(DSTRING *buf)
{
	ssize_t		ind;

	ind = dstrrchr(buf, ' ');
	return (dstr_serr(buf, ind + 1, buf->strlen));
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

static void		fill_buf(DSTRING **buf, const t_astr rez)
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
		if (ind != 0)
			dstr_insert_str((*buf), " ", (*buf)->strlen);
		dstr_insert_dstr((*buf), rez.strings[j++], (*buf)->strlen);
	}
}

int				reg_expr(DSTRING **buf, t_fl *fl)
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
	fl->tab = 0;
	fl->reg = 0;
	return ((*buf)->strlen);
}
