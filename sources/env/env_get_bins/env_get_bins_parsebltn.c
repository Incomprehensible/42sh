/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_bins_parsebltn.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:11:44 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/27 20:55:00 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "bltn.h"

// void	env_get_bins_parsebltn(t_darr *res, ENV *env)
// {
// 	int	i;
// 	int size;

// 	size = bltn_cnt_builtins();
// 	i = -1;
// 	while (++i < size)
// 	{
// 		if (env_get_bins_unq(res, bltns_str[i]))
// 		{
// 			(res)->strings[(res)->count] = dstr_new(bltns_str[i]);
// 			(res)->count++;
// 			env_get_bins_cmp_name(res, ft_strlen(bltns_str[i]));
// 		}
// 	}
	
// }

static void	ent_get_bins_treetravb(t_darr *res, t_avln *root)
{
	if (root)
	{
		if (env_get_bins_unq(res, (char *)root->key))
		{
			(res)->strings[(res)->count] = dstr_new((char *)root->key);
			(res)->count++;
			env_get_bins_cmp_name(res,\
									(res)->strings[(res)->count - 1]->strlen);
		}
		ent_get_bins_treetravb(res, root->left);
		ent_get_bins_treetravb(res, root->right);
	}
}

void		env_get_bins_parsebltn(t_darr *res, ENV *env)
{
	t_avln	*root;

	root = env->builtns->root;
	if (root)
		ent_get_bins_treetravb(res, root);
}
