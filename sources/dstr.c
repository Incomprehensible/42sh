/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dstr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 18:38:55 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/23 14:59:30 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dstring.h"
#include "libft.h"

ssize_t			dstr_del_char(DSTRING **src, const ssize_t n)
{
	DSTRING *begin;
	DSTRING *end;

	begin = dstr_slice((*src), 0, n);
	end = dstr_slice((*src), n + 1, (*src)->strlen);
	dstr_insert_dstr(begin, end, begin->strlen);
	dstr_del(src);
	dstr_del(&end);
	dstr_del(src);
	(*src) = begin;
	return (n - 1);
}

void			dstr_cutins_ch(DSTRING **dst, char ch, ssize_t ind)
{
	DSTRING *rez;

	rez = dstr_chr_swpc((*dst), ch, ind);
	dstr_del(dst);
	(*dst) = rez;
}

void					dstr_cutins_dstr(DSTRING **dst, DSTRING *src, size_t ind)
{
	DSTRING	*rez;

	rez = dstr_dstr_swpc((*dst), src, ind);
	dstr_del(dst);
	(*dst) = rez;
}

void			dstr_cutins_str(DSTRING **dst, char *src, ssize_t ind)
{
	DSTRING	*rez;

	rez = dstr_str_swpc((*dst), src, ind);
	dstr_del(dst);
	(*dst) = rez;
}

ssize_t			dstrrchr(const DSTRING *src, const int ch)
{
	ssize_t		rez;
	
	rez = src->strlen;
	while (rez--)
	{
		if (src->txt[rez] == ch)
			return (rez);
	}
	return (-1);
}

ssize_t			sh_dstr_iscmd(const DSTRING *str)
{
	ssize_t		i;
	ssize_t		space;
	ssize_t		smcln;
	
	space = dstrrchr(str, ' ');
	smcln = dstrrchr(str, ';');
	if (smcln != -1 && (i = smcln + 1))
	{
		while (ft_isspace(str->txt[i]))
			i++;
		if (i >= space + 1)
			return (-1);
	}
	i = 0;
	while (str->txt[i] == ' ')
		++i;
	if (str->strlen == 0 || space == -1 || space + 1 == i)
		return (-1);
	return (space + 1);
}
