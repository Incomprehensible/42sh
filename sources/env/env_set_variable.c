/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 16:12:50 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/17 22:52:06 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int		env_set_variable(const char *key, DSTRING *value, ENV *enviroment)
{
	t_avln	*node;

	if (!value)
		return (0);
	if (!(node = ft_avl_node((char *)key, (char *)value->txt,\
		ft_strlen((char *)value->txt) + 1)))
		return (-1);
	if (ft_avl_search(enviroment->globals, key) != 0)
	{
		if ((ft_avl_set(enviroment->globals, node)) < 0)
			return (-1);
	}
	else
	{
		if ((ft_avl_set(enviroment->locals, node)) < 0)
		// !BUG FIXED
			return (-1);
	}
	return (1);
}
