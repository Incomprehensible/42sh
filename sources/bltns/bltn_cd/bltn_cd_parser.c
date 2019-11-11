/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_cd_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 04:21:07 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/11 12:54:57 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int					bltn_cd_parser(ENV *env, t_dlist **path)
{
	char			*tmp_str;
	t_dyn_string	*str;
	t_dyn_string	*tmp;

	str = dstr_new((char *)ft_avl_search(env->locals, "PW")->content);
	while (str->strlen != 0)
	{
		if (ft_strrchr(str->txt, '/'))
		{
			tmp = dstr_slice_cut(&str, (ft_strrchr(str->txt, '/') - str->txt),\
										str->strlen);
			tmp_str = ft_strsub(tmp->txt, 1, tmp->strlen);
			if (!(*path))
				(*path) = ft_dlstnew(tmp_str, tmp->strlen);
			else
				ft_dlstunshift(&(*path), ft_dlstnew(tmp_str, tmp->strlen));
			dstr_del(&tmp);
			free(tmp_str);
		}
	}
	dstr_del(&str);
	return (1);
}
