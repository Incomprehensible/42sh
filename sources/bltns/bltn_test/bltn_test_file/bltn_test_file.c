/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_test_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 15:15:38 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/26 15:05:10 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_test.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int			bltn_t_file_err()
{
	sys_error_handler("test", E_TMARG, 0);
	return (2);
}

int					bltn_test_file(char **args, t_flags_file *fl)
{
	int				c_args;
	
	if (fl->s_notnull || fl->s_null)/////////STRINGS
		return (0);
	c_args = bltn_cd_countargs(args);
	if (c_args > 3 && (!ft_strequ(args[3], "-a") || !ft_strequ(args[3], "-o")))
		return (bltn_t_file_err());
	else if (c_args == 3)
		return (bltn_test_checkfile(args, fl));
	else//-a -o
	{
		
	}
	return (0);
}