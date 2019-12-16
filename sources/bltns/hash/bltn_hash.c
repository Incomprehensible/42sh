/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_hash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 02:25:21 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/16 21:04:19 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "bltn_hash.h"
#include "hstb.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"
#include "libftprintf.h"

int				hprs_args(t_bltn_ohs *ops, size_t *i, char **args)
{
	if (!args[*i])
		return (0);
	if (args[*i][0] == '-')
		return (2);
	ops->args = &(args[*i]);
	return (0);
}

int				hprs_flags(t_bltn_ohs *ops, size_t *i, char **args)
{
	size_t		j;

	j = 1;
	while (args[*i])
	{
		if (args[*i][j] == 'r' && j++)
			ops->rest = 1;
		else if (args[*i][j] == 'p' && j++)
		{
			if (args[*i][j] == '\0')
			{
				*i += 1;
				ft_strncpy(ops->path, args[*i], PATH_MAX);
				*i += 1;
				return (1);
			}
			else
				return (-1);
		}
		else if (args[*i][j] == '\0' && (*i += 1))
			return (1);
		else if (args[*i][j])
			return (-2);
	}
	return (0);
}

int				hprs_errors(int state, t_bltn_ohs *ops)
{
	if (state == -1)
		ops->err = "No path specified";
	else if (state == -2)
		ops->err = "Invalid option";
	return (0);
}

int				bltn_hash(char **args, ENV *env)
{
	t_bltn_ohs	ops;

	ops = hash_parse_args(args);
	ft_putendl("after parse");
	if (ops.err)
		return (hsprs_errhandler(ops.err, 2));
	ft_putendl("after errs");
	if (ops.rest && ops.path[0] == '\0')
		return (hsblt_reset(&ops, env));
	else if (ops.args && ops.args[0] && ops.path[0])
		return (hsblt_sethashes(&ops, env));
	else if (!ops.path[0])
		return (hsblt_printhashes(&ops, env));
	return (3);
}
