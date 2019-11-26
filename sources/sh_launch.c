/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_launch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 04:39:52 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/26 20:01:34 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_token.h"
#include "sh_req.h"
#include "sh_readline.h"
#include "env.h"
#include "executer.h"
#include "parser.h"
#include "dstring.h"
#include "bltn.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"
#include "ft_lbuffer.h"
#include "sh_tokenizer.h"

int			sys_core_set_init(ENV *env, char **argv)
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
