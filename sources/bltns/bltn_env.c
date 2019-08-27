/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 19:52:22 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/27 20:10:56 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int			bltn_env(ENV *env)
{
	char	**mas;
	int		i;

	i = -1;
	if (!env->globals)
		return (210);
	if (!(mas = ft_avl_tree_to_warr(env->globals, &avln_todstring_key_val)))
		return (210);
	while (mas[++i])
	{
		ft_putstr(mas[i]);
		ft_putchar('\n');
	}
	et_rm_warr(mas);
	return (0);
}
