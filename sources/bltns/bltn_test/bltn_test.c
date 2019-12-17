/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 13:31:37 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/17 16:07:40 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_test.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int			bltn_test_margs(char **args)
{
	if (bltn_cd_countargs(args) > 2)
		return (bltn_test_expr(args));
	else
		return (0);
	return (1);
}

int					bltn_test(char **args, ENV *env)
{
	t_flags_file	f_flgs;
	int				c_args;
	int				rez;

	rez = 0;
	ft_bzero(&f_flgs, sizeof(t_flags_file));
	if (!env->builtns)
		return (-128);
	if ((c_args = bltn_cd_countargs(args)) < 2)
		return (1);
	if (ft_strequ(args[1], "!"))
		f_flgs.neg = 1;
	if ((rez = bltn_test_prsfileflgs(&f_flgs,\
			(f_flgs.neg) ? args[2] : args[1])) == 1)
	{
		if (c_args < 3)
			return (f_flgs.neg);
		return (bltn_test_file(args, &f_flgs));
	}
	else if (rez == -2)
		return (bltn_test_margs(args));
	else if (rez == -1)
		return (bltn_test_expr(args));
	return (0);
}
