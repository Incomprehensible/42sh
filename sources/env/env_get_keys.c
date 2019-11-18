/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_keys.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:18:14 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/16 19:53:15 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static void	env_get_keys_treetrav(t_darr *res, t_avln *root, char *pre)
{
	if (root)
	{
		if (env_get_bins_unq(res, (char *)root->key))
		{
			(res)->strings[(res)->count] = dstr_new(pre);
			dstr_insert_str((res)->strings[(res)->count], (char *)root->key, \
				(res)->strings[(res)->count]->strlen);
			dstr_insert_str((res)->strings[(res)->count], pre[0] ? "}" : "", \
				(res)->strings[(res)->count]->strlen);
			(res)->count++;
			env_get_bins_cmp_name(res,\
									(res)->strings[(res)->count - 1]->strlen);
		}
		env_get_keys_treetrav(res, root->left, pre);
		env_get_keys_treetrav(res, root->right, pre);
	}
}


t_darr	env_get_keys(ENV *envp, char *pre)
{
	t_darr	res;
	t_avln	*root;

	root = envp->globals->root;
	res.allsize = 0;
	res.maxlen = 0;
	res.maxlen = 0;
	res.count = 0;
	env_get_keys_treetrav(&res, root, pre);
	root = envp->locals->root;
	env_get_keys_treetrav(&res, root, pre);
	return (res);
}
