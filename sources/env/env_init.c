/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 14:43:29 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/19 15:08:38 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "rms.h"
#include "bltn.h"
#include "aliases.h"

#include <stdio.h>

int			env_init_error(ENV *env, int errcode)
{
	if (errcode == 12)
		et_rm_clear_env(env);
	return (errcode);
}

void		free_spl(char ***spl)
{
	if (!(*spl))
		return ;
	if ((*spl)[0])
		free((*spl)[0]);
	if ((*spl)[1])
		free((*spl)[1]);
	if ((*spl))
		free((*spl));
}

int			env_init(char **envp, ENV *env)
{
	char	**spl;

	spl = NULL;
	if (!(env->globals = ft_avl_tree_create(free))\
		|| !(env->locals = ft_avl_tree_create(free))\
		|| !(env->builtns = ft_avl_tree_create(0))\
		|| !(env->funcs = ft_avl_tree_create(et_rm_func))\
		|| !(env->core = ft_avl_tree_create(free))\
		|| !(env->aliases = ft_avl_tree_create(free)))
		return (env_init_error(env, 12));
	while (*envp)
	{
		spl = ft_strsplit(*envp, '=');
		if ((ft_avl_set(env->globals, ft_avl_node(spl[0], (char *)spl[1],\
		spl[1] ? ft_strlen(spl[1]) + 1 : 0)) == -1))
		{
			free_spl(&spl);
			return (env_init_error(env, 12));
		}
		free_spl(&spl);
		envp++;
	}
	if (bltn_init(env) == -1)
		return (env_init_error(env, 12));
	return (1);
}
