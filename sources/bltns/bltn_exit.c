/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 13:55:49 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/20 19:21:07 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include "sh_readline.h"
#include "sys_tools/sys_tools.h"

int		make_exit(unsigned char ret, ENV *envr)
{
	size_t		l;

	l = g_jid;
	while (--l > 0)
	{
		if (g_ptab[l] != 0 && g_ptab[l]->mode == PS_M_BG \
		&& g_ptab[l]->state == PS_S_STP)
			return (sys_perror("exit: there are STOPPED jobs", 2, envr));
	}
	free_darr_re(g_histr.strings, g_histr.count + 1);
	exit(ret);
	return (0);
}

int		bltn_exit(char **args, ENV *env)
{
	int				exit_status;
	unsigned char	res;

	if (args[1] == 0)
		make_exit(0, env);
	else if (args[2] == 0)
	{
		if (!ft_isdigit(*args[1]) && *args[1] != '+' && *args[1] != '-')
			sys_error_handler("exit: argument is not numeric", 0, env);
		else
		{
			exit_status = ft_atoi(args[1]);
			res = (unsigned char)exit_status;
			make_exit(res, env);
		}
	}
	else
		sys_error_handler("exit: too many arguments", 0, env);
	return (1);
}
