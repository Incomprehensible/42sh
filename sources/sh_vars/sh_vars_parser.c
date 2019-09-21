/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_vars_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 12:57:24 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/17 13:11:19 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_vars.h"

static int	sh_parse_files(int argc, char **argv, t_envp *env, int n)
{
	int		i;

	i = -1;
	if (env->flgs.cmd)
		n++;
	while (n < argc)
	{
		if (env->flgs.cmd)
		{
			env->flg_c_string = dstr_new(argv[n]);
			return (1);
		}
		else
			env->files[++i] = dstr_new(argv[n]);
		n++;
	}
	env->files[++i] = NULL;
	return (0);
}

static int	sh_write_flag(char *str, t_envp *env)
{
	while (*str)
	{
		if (*str == 'c')
		{
			env->flgs.cmd = 1;
			return (1);
		}
		str++;
	}
	return (0);
}

int			sh_parser_argv(int argc, char **argv, t_envp *env)
{
	int i;

	i = 0;
	env->flgs.cmd = 0;
	while (++i < argc)
	{
		if (argv[i][0] == '-')
		{
			if ((sh_write_flag(++argv[i], env)) == 1)
				break ;
		}
		else
			break ;
	}
	return (sh_parse_files(argc, argv, env, i));
}
