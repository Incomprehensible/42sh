/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expr_action.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 16:40:30 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/17 16:09:46 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_test.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int	return_res(t_eq_args *exp, int res)
{
	dstr_del(&exp->secon_arg);
	dstr_del(&exp->first_arg);
	return ((exp->neg) ? !res : res);
}

static int	exp_strings(t_eq_args *exp)
{
	if (exp->acti == 1 && ft_strequ(exp->first_arg->txt, exp->secon_arg->txt))
		return (return_res(exp, 0));
	else if (exp->acti == 2 &&\
			!ft_strequ(exp->first_arg->txt, exp->secon_arg->txt))
		return (return_res(exp, 0));
	return (return_res(exp, 1));
}

static int	exp_dig_errs(t_eq_args *exp)
{
	sys_error_handler("test", E_INTEX, 0);
	return (return_res(exp, 2));
}

static int	exp_digits(t_eq_args *exp)
{
	int		first;
	int		sec;

	first = ft_atoi(exp->first_arg->txt);
	sec = ft_atoi(exp->secon_arg->txt);
	if ((first == 0 && !ft_strequ(exp->first_arg->txt, "0")) ||\
		(sec == 0 && !ft_strequ(exp->secon_arg->txt, "0")))
		return (exp_dig_errs(exp));
	if (exp->acti == 3 && first == sec)
		return (return_res(exp, 0));
	else if (exp->acti == 4 && first >= sec)
		return (return_res(exp, 0));
	else if (exp->acti == 5 && first > sec)
		return (return_res(exp, 0));
	else if (exp->acti == 6 && first <= sec)
		return (return_res(exp, 0));
	else if (exp->acti == 7 && first < sec)
		return (return_res(exp, 0));
	else if (exp->acti == 8 && first != sec)
		return (return_res(exp, 0));
	return (return_res(exp, 1));
}

int			test_exp_doact(t_eq_args *exp)
{
	if (exp->acti > 0 && exp->acti < 3)
		return (exp_strings(exp));
	else if (exp->acti > 2 && exp->acti < 9)
		return (exp_digits(exp));
	return (return_res(exp, 1));
}
