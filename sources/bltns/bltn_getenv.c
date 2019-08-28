/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_getenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 20:01:57 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/28 14:10:31 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int			bltn_getenv(char **args, ENV *env)
{
	t_avln	*node;
	int		i;
	int		ret;

	i = 0;
	ret = -1;
	while (args[++i])
	{
		if ((node = ft_avl_search(env->globals, args[i])) != 0)
		{
			ft_putstr((char*)node->content);
			ft_putstr("\n");
			ret = 0;
		}
		else if (ft_avl_search(env->locals, args[i]) != 0)
			return (242);
	}
	if (ret == -1)
		return (bltn_env(env));
	return (0);
}
