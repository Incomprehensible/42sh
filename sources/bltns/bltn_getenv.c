/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_getenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 20:01:57 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/27 20:08:05 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int			bltn_getenv(char *key, ENV *env)
{
	t_avln	*node;

	if ((node = ft_avl_search(env->globals, key)) != 0)
	{
		ft_putstr((char*)node->content);
		ft_putstr("\n");
		return (0);
	}
	else if (ft_avl_search(env->locals, key) != 0)
		return (242);
	return (bltn_env(env));
}
