/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_ops_log_eq.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:43:27 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/15 18:44:03 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_math/math_hidden.h"

long	mop_or(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) || math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_and(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = math_eval(ex1, envr) && math_eval(ex2, envr);
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_eq(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) == math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_neq(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) != math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}
