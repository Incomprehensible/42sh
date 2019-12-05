/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_unsetenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 20:11:12 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/05 18:02:42 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int		bltn_unsetenv(char **args, ENV *env)
{
	int	i;
	int	ret;

	i = 0;
	ret = 200;
	while (args[++i])
	{
		if (ft_avl_search(env->locals, args[i]) != 0)
			return (242);
		else
		{
			if (ft_avl_search(env->globals, args[i]) != 0)
			{
				if ((ft_avl_del(env->globals, args[i])) == -1)
					return (-1);
				ret = 0;
			}
		}
	}
	return (ret);
}
