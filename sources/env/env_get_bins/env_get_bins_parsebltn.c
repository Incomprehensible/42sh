/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_bins_parsebltn.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 17:11:44 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/22 20:07:40 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "bltn.h"

void	env_get_bins_parsebltn(t_darr *res)
{
	int	i;
	int size;

	size = bltn_cnt_builtins();
	i = -1;
	while (++i < size)
	{
		if (env_get_bins_unq(res, bltns_str[i]))
		{
			(res)->strings[(res)->count] = dstr_new(bltns_str[i]);
			(res)->count++;
			env_get_bins_cmp_name(res, ft_strlen(bltns_str[i]));
		}
	}
}
