/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_ops_lw_bg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:45:24 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/15 18:45:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_math/math_hidden.h"

long	mop_lw(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) < math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_bg(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) > math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}

long	mop_lweq(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) <= math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}


long	mop_bgeq(DSTRING *ex1, DSTRING *ex2, ENV *envr)
{
	long	res;

	res = (math_eval(ex1, envr) >= math_eval(ex2, envr));
	dstr_del(&ex1);
	dstr_del(&ex2);
	return (res);
}
