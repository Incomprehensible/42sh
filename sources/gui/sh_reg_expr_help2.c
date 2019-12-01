/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_reg_expr_help2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 22:05:43 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/29 22:12:06 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

static t_beend	cut_reg(char *str)
{
	int			i;
	int			fl;
	t_beend		rez;

	i = -1;
	fl = 0;
	rez.begin = 0;
	rez.end = -1;
	while (str[++i])
	{
		if (ft_memchr("*[?", str[i], 3))
			fl = 1;
		if (fl && ft_memchr(" /", str[i], 2))
		{
			rez.end = i;
			break ;
		}
		if (str[i] == ' ' && !(fl = 0))
			rez.begin = i + 1;
	}
	if (rez.end == -1)
		rez.end = i;
	return (rez);
}

t_buf			slicer_reg(DSTRING **buf)
{
	t_buf		new;
	t_beend		beend;

	new.buf = dstr_nerr((*buf)->txt);
	new.sub = NULL;
	new.dir = NULL;
	beend = cut_reg((*buf)->txt);
	new.begin = dstr_serr((*buf), 0, beend.begin);
	new.sub = dstr_scerr(buf, beend.begin, beend.end);
	new.end = dstr_serr((*buf), beend.begin, (*buf)->strlen);
	new.val = NULL;
	new.cut = beend.begin;
	new.slash = 0;
	return (new);
}
