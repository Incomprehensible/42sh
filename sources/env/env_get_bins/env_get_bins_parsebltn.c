/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_bins_parsebltn.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:11:44 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/11 13:10:33 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "bltn.h"

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
