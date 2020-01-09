/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 13:31:37 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/17 18:46:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_test.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int			bltn_test_margs(char **args, ENV *env)
{
	if (!env->builtns)
		return (-128);
	if (bltn_cd_countargs(args) > 2)
		return (bltn_test_expr(args));
	else
		return (0);
	return (1);
}

int					kostyl_test(char **args, int ac)
{
	if (ac == 2 && args[1][0])
		return (0);
	else if (ac == 2)
		return (1);
	return (0);
}

int					bltn_test(char **args, ENV *env)
{
	t_flags_file	f_flgs;
	int				c_args;
	int				rez;

	rez = 0;
	ft_bzero(&f_flgs, sizeof(t_flags_file));
	if ((c_args = bltn_cd_countargs(args)) <= 2)
		return (kostyl_test(args, c_args));
	ft_strequ(args[1], "!") ? f_flgs.neg = 1 : 0;
	if (f_flgs.neg && c_args == 3 && args[2][0] != '-')
		return (1);
	if ((rez = bltn_test_prsfileflgs(&f_flgs,\
			(f_flgs.neg) ? args[2] : args[1])) == 1)
	{
		if (c_args < 3)
			return (f_flgs.neg);
		return (bltn_test_file(args, &f_flgs));
	}
	else if (rez == -2)
		return (bltn_test_margs(args, env));
	else if (rez == -1)
		return (bltn_test_expr(args));
	return (0);
}
