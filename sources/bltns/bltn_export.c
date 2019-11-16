/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 14:51:48 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/16 15:24:11 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int				bltn_export(char **args, ENV *envr)
{
	int	i;
	int j;
	char *arg[256];

	i = 0;
	j = 0;
	arg[0] = ft_strdup("export");
	while (args[++i])
	{
		if (ft_strchr(args[i], '='))
			arg[++j] = ft_strdup(args[i]);
		else
		{
			if (ft_avl_search(envr->locals, args[i]) != 0)
				ft_avl_set(envr->globals, ft_avl_cut(envr->locals, args[i]));
		}
	}
	arg[++j] = NULL;
	bltn_setenv(arg, envr);
	j = -1;
	while (arg[++j])
		ft_strdel(&arg[j]);
	return (0);
}
