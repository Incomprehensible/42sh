/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_test_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 15:15:38 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/17 17:51:34 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_test.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int			bltn_t_file_err(void)
{
	sys_error_handler("test", E_TMARG, 0);
	return (2);
}

static int			bltn_test_string(char **args, t_flags_file *fl)
{
	if (fl->s_notnull && ft_strlen((fl->neg) ? args[3] : args[2]) != 0)
		return ((fl->neg) ? 1 : 0);
	else if (fl->s_null && ft_strlen((fl->neg) ? args[3] : args[2]) == 0)
		return ((fl->neg) ? 1 : 0);
	return ((fl->neg) ? 0 : 1);
}

int					bltn_test_file(char **args, t_flags_file *fl)
{
	int				c_args;

	c_args = bltn_cd_countargs(args);
	if ((c_args > 3 && !fl->neg) || (fl->neg && c_args != 4))
		return (bltn_t_file_err());
	if (fl->s_notnull || fl->s_null)
		return (bltn_test_string(args, fl));
	else if (c_args == 3 || (c_args == 4 && fl->neg))
		return (bltn_test_checkfile(args, fl));
	else
		return (bltn_t_file_err());
	return (0);
}
