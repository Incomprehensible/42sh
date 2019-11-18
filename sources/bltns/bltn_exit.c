/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 13:55:49 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/18 23:19:25 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include "sh_readline.h"
#include "sys_tools/sys_tools.h"

int	bltn_exit(char **args, ENV *env)
{
	int				exit_status;
	unsigned char	res;

	if (args[1] == 0)
		exit(0);
	else if (args[2] == 0)
	{
		if (!ft_isdigit(*args[1]) && *args[1] != '+' && *args[1] != '-')
			sys_error_handler("exit: argument is not numeric", 0, env);
		else
		{
			free_darr_re(g_histr.strings, g_histr.count);
			exit_status = ft_atoi(args[1]);
			res = (unsigned char)exit_status;
			exit(res);
		}
	}
	else
		sys_error_handler("exit: too many arguments", 0, env);
	return (1);
}
