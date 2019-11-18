/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_keys.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 19:56:03 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/11 13:11:05 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static void		env_get_keys_treetrav(t_darr *res, t_avln *root)
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
		env_get_keys_treetrav(res, root->left);
		env_get_keys_treetrav(res, root->right);
	}
}

t_darr			env_get_keys(ENV *envp)
{
	t_darr	res;
	t_avln	*root;

	root = envp->globals->root;
	res.allsize = 0;
	res.maxlen = 0;
	res.maxlen = 0;
	res.count = 0;
	env_get_keys_treetrav(&res, root);
	root = envp->locals->root;
	env_get_keys_treetrav(&res, root);
	return (res);
}
