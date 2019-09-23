/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_bins_parsefunc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:11:59 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/22 20:07:32 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static void	ent_get_bins_treetrav(t_darr *res, t_avln *root)
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
		ent_get_bins_treetrav(res, root->left);
		ent_get_bins_treetrav(res, root->right);
	}
}

void		env_get_bins_parefunc(t_darr *res, ENV *envp)
{
	t_avln	*root;

	root = envp->funcs->root;
	if (root)
		ent_get_bins_treetrav(res, root);
}
