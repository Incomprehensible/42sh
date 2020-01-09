/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_reg_expr_help2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 22:05:43 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/12/17 15:08:11 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

int				skip_math(char *str, int i)
{
	int			brackets;

	brackets = 0;
	while (str[i])
	{
		if (str[i] == ')' && str[i + 1] == ')' && !brackets)
			return (i + 2);
		if (str[i] == '(')
			brackets += 1;
		if (str[i] == ')')
			brackets -= 1;
		++i;
	}
	return (i);
}

static t_beend	cut_reg(char *str)
{
	int			i;
	int			fl;
	t_beend		rez;
	int			apof;
	int			dqoute;

	i = -1;
	fl = 0;
	rez.begin = 0;
	rez.end = -1;
	apof = 0;
	dqoute = 0;
	while (str[++i])
	{
		str[i] == '\'' ? apof = !apof : 0;
		str[i] == '\"' ? dqoute = !dqoute : 0;
		str[i] == '(' && str[i + 1] == '(' ? i = skip_math(str, i + 2) : 0;
		ft_memchr("*[?", str[i], 3) ? fl = 1 : 0;
		if (fl && ft_memchr(" /", str[i], 2) && ((rez.end = i) || 1))
			break ;
		if (str[i] == ' ' && !(fl = 0))
			rez.begin = i + 1;
	}
	(rez.end == -1) ? rez.end = i : 0;
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
