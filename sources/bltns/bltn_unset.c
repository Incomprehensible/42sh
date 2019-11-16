/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 15:05:05 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/16 14:07:05 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int	bltn_unset_parseflags(char **args, t_flags *flgs)
{
	int	i;
	int j;

	i = 0;
	while (args[++i])
	{
		if (args[i][0] == '-')
		{
			j = 0;
			while (args[i][++j])
			{
				if (args[i][j] == 'f')
					flgs->first = 1;	
				else if (args[i][j] == 'v')
					flgs->second = 1;
				else
					return (-1);
			}
		}
		else
			break;
	}
	return (i);
}

static int	bltn_unset_varfunc(char *args, ENV *env)
{
	if (ft_avl_search(env->globals, args) != 0)
	{
		if ((ft_avl_del(env->globals, args)) != -1)
			return (1);
	}
	else if (ft_avl_search(env->locals, args) != 0)
	{
		if ((ft_avl_del(env->locals, args)) != -1)
			return (1);
	}
	else if (ft_avl_search(env->funcs, args) != 0)
	{
		if ((ft_avl_del(env->funcs, args)) != -1)
			return (1);
	}
	return (0);
}

int			bltn_unset(char **args, ENV *envr)
{
	t_flags	flgs;
	int		i;

	i = 0;
	flgs.first = 0;
	flgs.second = 0;
	if ((i = bltn_unset_parseflags(args, &flgs)) != -1)
	{
		if (flgs.first == 1 && flgs.second == 1)
			sys_error_handler("unset", E_UNFLG, 0);	
		else
		{
			i--;
			while (args[++i])
			{
				if (flgs.first)
					env_unset_func(args[i], envr);
				else if (flgs.second)
					env_unset_variable(args[i], envr);
				else if (!flgs.first && !flgs.second)
					bltn_unset_varfunc(args[i], envr);
			}
		}
	}
	else
		sys_error_handler("unset", E_FLGER, 0);
	return (0);
}
