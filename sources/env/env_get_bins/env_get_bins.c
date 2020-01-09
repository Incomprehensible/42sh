/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_bins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 15:22:08 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/14 00:07:16 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

void		env_get_bins_cmp_name(t_darr *res, int namelen)
{
	if ((size_t)namelen > res->maxlen)
		(res)->maxlen = (size_t)namelen;
	(res)->allsize += (size_t)namelen;
}

int			env_get_bins_unq(t_darr *res, char *str)
{
	size_t	i;

	i = (size_t)-1;
	while (++i < res->count)
		if (ft_strcmp(res->strings[i]->txt, str) == 0)
			return (0);
	return (1);
}
