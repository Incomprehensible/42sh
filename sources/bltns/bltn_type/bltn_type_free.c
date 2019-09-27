/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_type_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 19:22:17 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/27 17:55:17 by fnancy           ###   ########.fr       */
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
	int		i;

	i = -1;
	while (++i < res->count)
		dstr_del(&res->strings[i]);
}
