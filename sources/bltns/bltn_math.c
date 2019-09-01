/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_math.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 17:42:46 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/01 21:30:37 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bltn.h"

long	math_eval(DSTRING *expr);

long	mop_add(DSTRING *ex1, DSTRING *ex2)
{
	long	res;

	res = math_eval(ex1) + math_eval(ex2);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_mlt(DSTRING *ex1, DSTRING *ex2)
{
	long	res;

	res = math_eval(ex1) * math_eval(ex2);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_div(DSTRING *ex1, DSTRING *ex2)
{
	long	res;

	res = math_eval(ex1) / math_eval(ex2);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_suj(DSTRING *ex1, DSTRING *ex2)
{
	long	res;

	res = math_eval(ex1) - math_eval(ex2);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_mod(DSTRING *ex1, DSTRING *ex2)
{
	long	res;

	res = math_eval(ex1) % math_eval(ex2);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_inc(DSTRING *ex)
{
	long	res;

	res = 1 + math_eval(ex);
	dstr_del(&ex);
	return (res);
}

long	mop_dec(DSTRING *ex)
{
	long	res;

	res = -1 + math_eval(ex);
	dstr_del(&ex);
	return (res);
}

long	math_eval(DSTRING *expr)
{
	size_t		ind;

	if ((ind = dstr_search_ch(expr, '+')) != SIZE_T_MAX && expr->txt[ind + 1] != '+')
		return (mop_add(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX)));
	else if ((ind = dstr_search_ch(expr, '-')) != SIZE_T_MAX && expr->txt[ind + 1] != '-')
		return (mop_suj(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX)));
	else if ((ind = dstr_search_ch(expr, '/')) != SIZE_T_MAX)
		return (mop_div(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX)));
	else if ((ind = dstr_search_ch(expr, '%')) != SIZE_T_MAX)
		return (mop_mod(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX)));
	else if ((ind = dstr_search_ch(expr, '*')) != SIZE_T_MAX)
		return (mop_mlt(dstr_slice(expr, 0, ind), dstr_slice(expr, ind + 1, SSIZE_T_MAX)));
	else if ((ind = dstr_search_str(expr, "++")) != SIZE_T_MAX)
		return (mop_inc(dstr_slice(expr, ind + 2, SSIZE_T_MAX)));
	else if ((ind = dstr_search_str(expr, "--")) != SIZE_T_MAX)
		return (mop_dec(dstr_slice(expr, ind + 2, SSIZE_T_MAX)));
	return (ft_atoll_base(expr->txt, 10));
}

int		bltn_math(char **args, ENV *env)
{
	DSTRING		*expr;
	long		res;

	expr = dstr_new(args[1]);
	res = math_eval(expr);
	dstr_del(&expr);
	return ((int)res);
}