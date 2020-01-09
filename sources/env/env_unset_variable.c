/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset_variable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 16:40:21 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/27 16:45:10 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int		env_unset_variable(const char *key, ENV *enviroment)
{
	if (ft_avl_search(enviroment->globals, key) != 0)
	{
		if ((ft_avl_del(enviroment->globals, key)) != -1)
			return (1);
	}
	else if (ft_avl_search(enviroment->locals, key) != 0)
	{
		if ((ft_avl_del(enviroment->locals, key)) != -1)
			return (1);
	}
	return (0);
}
