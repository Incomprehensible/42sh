/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 13:31:37 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/26 16:54:23 by fnancy           ###   ########.fr       */
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
	if (!env->builtns)
		return (-128);
	if ((c_args = bltn_cd_countargs(args)) < 2)
		return (1);
	if ((rez = bltn_test_prsfileflgs(&f_flgs, args[1])) == 1)
	{
		if (c_args < 3)
			return (0);
		return (bltn_test_file(args, &f_flgs));
	}
	else if (rez == -2)
		return (bltn_test_margs(args));
	else if (rez == -1)
		return (bltn_test_expr(args));
	return (0);
}
