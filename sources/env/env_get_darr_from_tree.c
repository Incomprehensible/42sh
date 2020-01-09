/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_darr_from_tree.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 19:34:46 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/14 19:34:54 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sys_tools/sys_tools.h"

static void		ft_avl_bltn_darr(DSTRING *sub, t_darr *darr, t_avln *node)
{
	DSTRING	*dstring;

	if (!node || !sub)
		return ;
	if (darr->count >= S_DARR_STRINGS)
		return ;
	ft_avl_bltn_darr(sub, darr, node->left);
	if (ft_strnequ(sub->txt, node->key, sub->strlen))
	{
		dstring = dstr_new(node->key);
		if (dstring)
		{
			darr->strings[darr->count] = dstring;
			darr->count += 1;
			darr->maxlen < (size_t)dstring->strlen ? \
				darr->maxlen = dstring->strlen : 0;
			darr->allsize += dstring->strlen;
		}
	}
	ft_avl_bltn_darr(sub, darr, node->right);
}

t_darr			get_overlap_darr(DSTRING *sub, t_avl_tree *avl)
{
	t_darr	darr;

	ft_bzero(&darr, sizeof(t_darr));
	if (!sub || !avl)
		return (darr);
	ft_avl_bltn_darr(sub, &darr, avl->root);
	return (darr);
}
