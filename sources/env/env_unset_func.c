/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 14:03:26 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/16 14:10:46 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int		env_unset_func(const char *key, ENV *enviroment)
{
	if (ft_avl_search(enviroment->funcs, key) != 0)
	{
		if ((ft_avl_del(enviroment->funcs, key)) != -1)
			return (1);
	}
	return (0);
}
