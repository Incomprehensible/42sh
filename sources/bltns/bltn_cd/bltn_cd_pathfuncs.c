/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_cd_pathfuncs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 07:14:50 by fnancy            #+#    #+#             */
/*   Updated: 2019/10/16 02:41:35 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

void		et_rm_str(void *s, size_t l)
{
	l = 0;
	free(s);
}

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
	ft_dlst_delf(&(*path), sizeof((*path)) / sizeof(t_dlist *), et_rm_str);
}