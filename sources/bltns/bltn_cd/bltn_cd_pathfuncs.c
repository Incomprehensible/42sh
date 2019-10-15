/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_cd_pathfuncs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 07:14:50 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/29 15:24:12 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

DSTRING		*bltn_cd_pathtostr(t_dlist *path)
{
	DSTRING	*res;
	char	*val;

	val = (char *)path->content;
	if (val[0] != '/')
		res = dstr_new("/");
	else
	{
		res = dstr_new(val);
		path = path->next;
	}
	while (path)
	{
		if (ft_strlen((char *)path->content) != 0)
			dstr_insert_str(res, (char *)path->content, res->strlen);
		if (path->next != NULL)
			dstr_insert_ch(res, '/', res->strlen);
		path = path->next;
	}
	return (res);
}

void		bltn_cd_destroy_path(t_dlist **path)
{
	ft_dlst_delf(&(*path), sizeof((*path)) / sizeof(t_dlist *), free);
}