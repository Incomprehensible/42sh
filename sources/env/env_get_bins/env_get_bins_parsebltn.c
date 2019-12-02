/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_bins_parsebltn.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:11:44 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/02 20:32:45 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "bltn.h"

static void	ent_get_bins_treetravb(t_darr *res, t_avln *root, DSTRING *sb)
{
	if (root)
	{
		ent_get_bins_treetravb(res, root->left, sb);
		if (ft_strnequ(sb->txt, root->key, sb->strlen) && \
		env_get_bins_unq(res, (char *)root->key))
		{
			(res)->strings[(res)->count] = dstr_new((char *)root->key);
			(res)->count++;
			env_get_bins_cmp_name(res,\
									(res)->strings[(res)->count - 1]->strlen);
		}
		ent_get_bins_treetravb(res, root->right, sb);
	}
}

void		env_get_bins_parsebltn(t_darr *res, ENV *env, DSTRING *sb)
{
	t_avln	*root;

	root = env->builtns->root;
	if (root)
		ent_get_bins_treetravb(res, root, sb);
}
