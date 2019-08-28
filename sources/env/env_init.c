/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 14:43:29 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/28 13:11:19 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "rms.h"

static int	env_init_error(ENV *env, int errcode)
{
	if (errcode == 12)
		et_rm_clear_env(env);
	return (errcode);
}

static void	free_spl(char ***spl)
{
	free((*spl)[0]);
	free((*spl)[1]);
	free((*spl));
}

int			env_init(int argc, char **argv, char **envp, ENV *env)
{
	char	**spl;

	spl = NULL;
	if (!(env->globals = ft_avl_tree_create(free))\
		|| !(env->locals = ft_avl_tree_create(free))\
		|| !(env->builtns = ft_avl_tree_create(free))\
		|| !(env->funcs = ft_avl_tree_create(free)))
		return (env_init_error(env, 12));
	while (*envp)
	{
		spl = ft_strsplit(*envp, '=');
		if ((ft_avl_set(env->globals, ft_avl_node(spl[0], (char *)spl[1],\
					ft_strlen(spl[1]) + 1)) == -1))
		{
			free_spl(&spl);
			return (env_init_error(env, 12));
		}
		free_spl(&spl);
		envp++;
	}
	return (1);
}
