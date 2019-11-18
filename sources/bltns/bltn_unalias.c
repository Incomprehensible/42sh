/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_unalias.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 18:36:48 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/11 13:09:52 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aliases.h"
#include "bltn.h"

int		bltn_unalias(char **args, ENV *env)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if ((ft_avl_search(env->aliases, args[i])) != 0)
		{
			if ((ft_avl_del(env->aliases, args[i])) == -1)
				return (-1);
			if (alias_add(env) == -1)
				return (-1);
		}
	}
	return (1);
}
