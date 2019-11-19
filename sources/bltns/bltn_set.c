/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_set.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 04:22:57 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 15:07:20 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int		b_set_put_warr(char **warr)
{
	size_t	i;

	i = 0;
	if (!warr)
		return (2);
	while (warr[i])
		ft_putendl(warr[i++]);
	return (0);
}

int		b_set_put_vars(ENV *env)
{
	char	**mas[3];
	size_t	i;

	i = 0;
	if (!(mas[0] = ft_avl_tree_to_warr(env->globals, &avln_todstring_key_val)))
		return (210);
	if (!(mas[1] = ft_avl_tree_to_warr(env->locals, &avln_todstring_key_val)))
		return (210);
	if (!(mas[2] = ft_avl_tree_to_warr(env->core, &avln_todstring_key_val)))
		return (210);
	while (i < 3)
		if (mas[i++] == 0)
			return (2);
	i = 0;
	while (i < 3)
	{
		b_set_put_warr(mas[i]);
		ft_putendl("");
		et_rm_warr(mas[i++]);
	}
	return (0);
}

int		bltn_set(char **argv, ENV *env)
{
	if (argv[1] == 0)
		return (b_set_put_vars(env));
	return (0);
}
