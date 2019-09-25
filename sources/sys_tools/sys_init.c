/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 09:00:27 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/25 16:48:07 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_errors.h"

#include "ft_mem.h"
#include "env.h"

int			sys_core_set_init(ENV *env, char **argv, int argc)
{
	int			i;
	char		*key;
	char		*argc_val;

	i = -1;
	key[0] = '0';
	key[1] = 0;
	if (!(argc_val = ft_itoa(argc)) || \
	(env_core_set("#", argc_val, env->core) < 0))
		sys_fatal_memerr("MALLOC CALL FAILED, CORE VARIABLES LOST!");
	while (++i < argc)
	{
		key = ft_itoa(i);
		if (!key || env_core_set((char *)key, argv[i], env->core) < 0)
			sys_fatal_memerr("MALLOC CALL FAILED, CORE VARIABLES LOST!");
		free(key);
	}
	free(argc_val);
	return (0);
}

int			sys_var_init(ENV *env, char **argv, int argc)
{
	DSTRING		*val;

	val = dstr_new(SHELL_NAME_STD);
	if ((env_set_variable(SH_VAR_SHNAME, val, env)) <= 0)
		sys_fatal_memerr("MALLOC CALL FAILED!");
	dstr_del(&val);
	prs_set_pid(env);
	if ((sys_core_set_init(env, argv, argc)) < 0)
		sys_fatal_memerr("MALLOC CALL FAILED, CORE VARIABLES LOST!");
	return (0);
}

int			sys_init(void)
{
	ft_bzero(sys_pipes, SYS_PIPES_SIZE * sizeof(char));
	return (0);
}
