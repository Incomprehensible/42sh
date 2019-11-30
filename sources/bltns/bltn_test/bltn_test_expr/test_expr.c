/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:34:46 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/26 16:47:45 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_test.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int		expt_err(t_eq_args *exp)
{
	sys_error_handler("test", E_BNEXP, 0);
	dstr_del(&exp->first_arg);
	dstr_del(&exp->secon_arg);
	return (1);
}

static int		get_action(char *str, t_eq_args *exp)
{
	if (ft_strequ(str, "="))
		exp->acti = 1;
	else if (ft_strequ(str, "!="))
		exp->acti = 2;
	else if (ft_strequ(str, "-eq"))
		exp->acti = 3;
	else if (ft_strequ(str, "-ge"))
		exp->acti = 4;
	else if (ft_strequ(str, "-gt"))
		exp->acti = 5;
	else if (ft_strequ(str, "-le"))
		exp->acti = 6;
	else if (ft_strequ(str, "-lt"))
		exp->acti = 7;
	else if (ft_strequ(str, "-ne"))
		exp->acti = 8;
	else if (ft_strequ(str, "-ef"))
		exp->acti = 9;
	else if (ft_strequ(str, "-nt"))
		exp->acti = 10;
	else if (ft_strequ(str, "-ot"))
		exp->acti = 11;
	else
		return (-1);
	return (1);
}

int				bltn_test_makeexpr(char **argv)
{
	t_eq_args	exp;

	exp.acti = 0;
	exp.first_arg = dstr_new(argv[1]);
	exp.secon_arg = dstr_new(argv[3]);
	if (get_action(argv[2], &exp) == -1)
		return (expt_err(&exp));
	return (test_exp_doact(&exp));
}