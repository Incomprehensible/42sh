/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_unsetenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 20:11:12 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/27 20:15:53 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int	bltn_unsetenv(char *key, ENV *env)
{
	if (ft_avl_search(env->locals, key) != 0)
		return (242);
	else
	{
		if (ft_avl_search(env->globals, key) != 0)
		{
			if (!(ft_avl_del(env->globals, key)))
				return (-1);
			return (0);
		}
	}
	return (200);
}
