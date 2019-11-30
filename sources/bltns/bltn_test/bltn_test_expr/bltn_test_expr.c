/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_test_expr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 15:15:55 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/26 17:33:36 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_test.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int			bltn_t_expr_err(int i)
{
	if (i == 1)
		sys_error_handler("test", E_TMARG, 0);
	if (i == 2)
		sys_error_handler("test", E_MFLTS, 0);
	return (2);
}

int					bltn_test_expr(char **args)
{
	int				c_args;
	
	c_args = bltn_cd_countargs(args);
	if (c_args == 3)
		return (bltn_t_expr_err(2));
	if (c_args > 4 && (!ft_strequ(args[4], "-a") || !ft_strequ(args[4], "-o")))
		return (bltn_t_expr_err(1));
	else if (c_args == 4)
		return (bltn_test_makeexpr(args));
	else //-a -o
	{
		
	}
	return (1);
}