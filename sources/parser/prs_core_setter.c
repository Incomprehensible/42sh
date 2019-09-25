/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_core_setter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 21:54:42 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/25 16:45:15 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_errors.h"

int			env_core_set(char *key, char *value, t_avl_tree *core)
{
	t_avln	*node;

	if (!(node = ft_avl_node(key, value, ft_strlen(value) + 1)))
		return (-1);
	if ((ft_avl_set(core, node)) < 0)
		return (-1);
	return (0);
}

int			prs_set_last_status(int *status, ENV *envr)
{
	char	*val;

	val = ft_itoa(*status);
	if (!val || (env_core_set("?", val, envr->core)< 0))
		sys_fatal_memerr("MALLOC FAILED, CORE VARIABLES LOST!");
	free(val);
	return (0);
}

int			prs_set_pid(ENV *envr)
{
	char	*val;
	pid_t	pid;

	pid = getpid();
	val = ft_itoa((int)pid);
	if (!val || (env_core_set("$", val, envr->core)< 0))
		sys_fatal_memerr("MALLOC FAILED, CORE VARIABLES LOST!");
	free(val);
	return (0);
}
