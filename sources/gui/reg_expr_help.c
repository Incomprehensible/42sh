/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reg_expr_help.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 00:58:52 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/09/22 16:29:30 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

DSTRING			*add_slash(char *str, DSTRING *reg)
{
	DSTRING		*rez;

	rez = dstr_nerr(str);
	if (reg->txt[reg->strlen - 1] == '/')
		dstr_insert_str(rez, "/", rez->strlen);
	return (rez);
}

char			chek_astr(const DSTRING *reg)
{
	int			i;

	i = -1;
	while (reg->txt[++i])
		if (reg->txt[i] == '*' || reg->txt[i] == '?' || reg->txt[i] == '[')
			return (0);
	return (1);
}

char			is_strdot(const char *path)
{
	if (ft_strlen(path) == 1 && path[0] == '.')
		return (0);
	if (ft_strlen(path) == 2 && path[1] == '.')
		return (0);
	return (1);
}

DSTRING			*join_reg(DSTRING *n_dir, DSTRING *cmp, const char fl)
{
	DSTRING		*rez;

	if (fl)
	{
		rez = dstr_nerr(n_dir->txt);
		if (rez->txt[rez->strlen - 1] != '/')
			dstr_insert_str(rez, "/", rez->strlen);
		dstr_insert_dstr(rez, cmp, n_dir->strlen);
		return (rez);
	}
	rez = dstr_nerr(cmp->txt);
	return (rez);
}

char			cmp_dirreg(DSTRING *n_dir, DSTRING *cmp,\
						DSTRING *reg, const char fl)
{
	DSTRING		*tmp;

	if (fl)
	{
		tmp = dstr_nerr(n_dir->txt);
		if (tmp->txt[tmp->strlen - 1] != '/')
			dstr_insert_str(tmp, "/", n_dir->strlen);
		dstr_insert_dstr(tmp, cmp, tmp->strlen);
		if (nmatch(tmp->txt, reg->txt))
		{
			dstr_del(&tmp);
			return (1);
		}
		dstr_del(&tmp);
	}
	else if (nmatch(cmp->txt, reg->txt))
		return (1);
	return (0);
}
