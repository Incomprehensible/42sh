/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_cd_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 04:21:07 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/19 02:02:23 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int					bltn_cd_parser(ENV *env, t_dlist **path)
{
	char			*tmp_str;
	t_dyn_string	*str;
	t_dyn_string	*tmp;

	env = 0;
	str = dstr_new(pwd->txt);
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
