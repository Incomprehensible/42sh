/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_type_alias_check.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 10:33:49 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/27 17:55:36 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn_type.h"

int			bltn_type_check_alias(char *str, DSTRING **val, ENV *env)
{
	char	*bin;

	if (ft_avl_search(env->aliases, str) != 0)
	{
		bin = (char *)ft_avl_search(env->aliases, str)->content;
		if (ft_strchr(bin, ' ') != NULL)
		{
			bin = ft_strsub(bin, 0, ft_strchr(bin, ' ') - bin);
			(*val) = dstr_new(bin);
			free(bin);
		}
		else
			(*val) = dstr_new(bin);
		return (1);
	}
	return (0);
}
