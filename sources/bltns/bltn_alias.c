/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_alias.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 15:07:08 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/13 12:33:06 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aliases.h"

static int	bltn_alias_showals(ENV *env)
{
	char	**mas;
	int		i;

	i = -1;
	if (!(mas = ft_avl_tree_to_warr(env->aliases, &avln_todstring_key_val)))
		return (-1);
	while (mas[++i])
	{
		ft_putstr(mas[i]);
		ft_putchar('\n');
	}
	et_rm_warr(mas);
	return (1);
}

int			bltn_alias(char **args, ENV *env)
{
	int		i;
	char	**spl;

	i = 0;
	if (!args[1])
		if (bltn_alias_showals(env) == -1)
			return (env_init_error(env, 12));
	while (args[++i])
	{
		spl = ft_strsplit(args[i], '=');
		if (!spl[1])
			return (-1);
		if ((ft_avl_set(env->aliases, ft_avl_node(spl[0], (char *)spl[1],\
					ft_strlen(spl[1]) + 1)) == -1))
		{
			free_spl(&spl);
			return (env_init_error(env, 12));
		}
		if (alias_add(env) == -1)
		{
			free_spl(&spl);
			return (env_init_error(env, 12));
		}
		free_spl(&spl);
	}
	return (1);
}
