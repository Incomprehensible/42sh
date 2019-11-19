/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 19:52:22 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/19 04:32:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int			bltn_env(char **args, ENV *env)
{
	char	**mas;
	int		i;

	if (!env->globals)
		return (210);
	if (bltn_cd_countargs(args) > 1)
		bltn_setenv(args, env);
	i = -1;
	if (!(mas = ft_avl_tree_to_warr(env->globals, &avln_todstring_key_val)))
		return (210);
	while (mas[++i])
		ft_putendl(mas[i]);
	et_rm_warr(mas);
	return (0);
}
