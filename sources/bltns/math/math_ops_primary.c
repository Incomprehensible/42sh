/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_ops_primary.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:48:05 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/15 18:48:21 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_math/math_hidden.h"

long	mop_add(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) + math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_mlt(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) * math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_div(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) / math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_suj(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) - math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_mod(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) % math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}
