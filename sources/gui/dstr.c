/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dstr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 18:38:55 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/09/01 19:16:36 by gdaemoni         ###   ########.fr       */
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

void			dstr_cutins_dstr(DSTRING **dst, DSTRING *src, size_t ind)
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

	if (!src)
		return (-1);
	rez = src->strlen;
	while (rez--)
	{
		if (src->txt[rez] == ch)
			return (rez);
	}
	return (-1);
}
