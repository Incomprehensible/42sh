/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 16:12:50 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/19 15:08:23 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int		env_set_variable(const char *key, DSTRING *value, ENV *enviroment)
{
	t_avln	*node;

	if (!value)
		return (0);
	if (!(node = ft_avl_node((char *)key, (char *)value->txt,\
		value->strlen + 1)))
		return (-1);
	if (ft_avl_search(enviroment->globals, key) != 0)
	{
		if ((ft_avl_set(enviroment->globals, node)) < 0)
			return (-1);
	}
	else
	{
		if ((ft_avl_set(enviroment->locals, node)) < 0)
			return (-1);
	}
	return (1);
}
