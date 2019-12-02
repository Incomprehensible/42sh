/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_bins_parsefunc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:11:59 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/02 20:35:07 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static void	ent_get_bins_treetrav(t_darr *res, t_avln *root, DSTRING *sb)
{
	if (root)
	{
		ent_get_bins_treetrav(res, root->left, sb);
		if (ft_strnequ(sb->txt, root->key, sb->strlen) && \
			env_get_bins_unq(res, (char *)root->key))
		{
			(res)->strings[(res)->count] = dstr_new((char *)root->key);
			(res)->count++;
			env_get_bins_cmp_name(res,\
									(res)->strings[(res)->count - 1]->strlen);
		}
		ent_get_bins_treetrav(res, root->right, sb);
	}
}

void		env_get_bins_parefunc(t_darr *res, ENV *envp, DSTRING *sb)
{
	t_avln	*root;

	root = envp->funcs->root;
	if (root)
		ent_get_bins_treetrav(res, root, sb);
}
