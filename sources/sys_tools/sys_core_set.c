/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_core_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 22:48:13 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/27 22:54:47 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "sys_tools/sys_errors.h"

int				sys_core_set_init(ENV *env, char **argv)
{
	int		i;
	int		j;
	char	*key;
	char	*argc_val;

	i = -1;
	j = 0;
	while (argv[++i])
	{
		if (argv[i][0] == '-')
			continue ;
		key = ft_itoa(j);
		if (!key || env_core_set((char *)key, argv[i], env->core) < 0)
			sys_fatal_memerr("MALLOC CALL FAILED, CORE VARIABLES LOST!");
		free(key);
		++j;
	}
	if (!(argc_val = ft_itoa(j)) || \
	(env_core_set("#", argc_val, env->core) < 0))
		sys_fatal_memerr("MALLOC CALL FAILED, CORE VARIABLES LOST!");
	free(argc_val);
	return (0);
}
