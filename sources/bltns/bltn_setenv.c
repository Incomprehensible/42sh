/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_setenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:46:16 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/27 19:56:26 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int			bltn_setenv(char *key, char *value, ENV *env)
{
	t_avln	*node;

	if (!value || value[0] == '\0')
		value = ft_strdup(" ");
	if (ft_avl_search(env->locals, key) != 0)
		return (242);
	else
	{
		if (!(node = ft_avl_node(key, value, ft_strlen(value))))
			return (210);
		if (!(ft_avl_set(env->globals, node)))
			return (210);
	}
	return (0);
}
