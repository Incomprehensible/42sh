/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_type_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 19:22:17 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/19 02:01:33 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_type.h"

int			bltn_type_freespl(char ***spl)
{
	int		i;

	i = -1;
	if (*spl)
	{
		while ((*spl)[++i])
			free((*spl)[i]);
		free((*spl));
	}
	return (0);
}

void		bltn_type_freedstrings(t_darr *res)
{
	size_t		i;

	i = (size_t)-1;
	while (++i < res->count)
		dstr_del(&res->strings[i]);
}
