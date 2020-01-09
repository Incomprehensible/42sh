/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_reg_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 21:42:47 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/29 21:42:43 by gdaemoni         ###   ########.fr       */
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

int				is_add_in_reg(DSTRING *str)
{
	int			i;

	i = str->strlen;
	if (str->txt[0] == '.')
		return (1);
	while (--i > -1)
	{
		if (ft_memchr("*?[", str->txt[i], 4))
			return (1);
		if (str->txt[i] == '/' && str->txt[i + 1] == '.')
			return (1);
		if (str->txt[i] == '/')
			return (0);
	}
	return (0);
}

DSTRING			*get_dstr_rez(t_astr rez)
{
	DSTRING		*reg;
	size_t		i;

	i = -1;
	reg = dstr_nerr("");
	while (++i < rez.count)
	{
		if (is_add_in_reg(rez.strings[i]) && rez.count != 1)
			continue ;
		check_spec_symbols(&rez.strings[i]);
		dstr_insert_dstr(reg, rez.strings[i], reg->strlen);
		if (i + 1 != rez.count)
			dstr_insert_ch(reg, ' ', reg->strlen);
	}
	return (reg);
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
