/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_source.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 23:33:26 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/27 23:37:15 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include "sh_req.h"

int				bltn_source(char **args, ENV *envr)
{
	t_avl_tree	*tmp;
	int			status;

	if (args[1] == 0)
	{
		sys_error_message("source: argument file required\n"
		"Usage: source filename [arguments]", 0);
		return (2);
	}
	tmp = envr->core;
	if (!(envr->core = ft_avl_tree_create(free)))
	{
		sys_error_message("source: Failed allocation", 0);
		envr->core = tmp;
		return (255);
	}
	sys_core_set_init(envr, args + 1);
	if (!(SYS_TCH_F & sys_touch_file(args[1])) && (status = 2))
		sys_error_handler(args[1], E_FOPEF, envr);
	else
		status = sh_do_src(args[1], envr);
	ft_avl_tree_free(envr->core);
	envr->core = tmp;
	return (status);
}
