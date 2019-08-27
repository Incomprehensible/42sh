/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 15:12:12 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/27 16:52:29 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

DSTRING		*env_get_variable(const char *key, ENV *enviroment)
{
	DSTRING	*res;
	t_avln	*node;

	if ((node = ft_avl_search(enviroment->locals, key)) != NULL)
	{
		if (!(res = dstr_new((char *)node->content)))
			res = NULL;
	}
	else if ((node = ft_avl_search(enviroment->globals, key)) != NULL)
	{
		if (!(res = dstr_new((char *)node->content)))
			res = NULL;
	}
	else
	{
		if (!(res = dstr_new("\0")))
			res = NULL;
	}
	return (res);
}
