/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dstr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 18:38:55 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/17 19:11:38 by gdaemoni         ###   ########.fr       */
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
	char		*points[2];

	points[0] = (char *)src->txt;
	points[1] = ft_strrchr(src->txt, (char)ch);

	return (points[1] - points[0]);
}

ssize_t			sh_dstr_iscmd(const DSTRING *str)
{
	size_t		i;

	i = 0;
	if (!str->txt)
		return (-1);
	while (str->txt[i] == ' ')
		++i;
	if (!ft_isalpha(str->txt[i]))
		return (-1);
	while (ft_isalpha(str->txt[i]))
		++i;
	while (str->txt[i] == ' ')
		++i;
	if (ft_isalpha(str->txt[i]) || str->txt[i] == '/' || str->txt[i] == '.')
		return (i);
	return (-1);
}
