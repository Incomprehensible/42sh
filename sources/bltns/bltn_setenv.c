/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_setenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:46:16 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/28 16:08:36 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

static void	free_spl(char ***str)
{
	if ((*str)[0])
		free((*str)[0]);
	if ((*str)[1])
		free((*str)[1]);
	free((*str));
}

static int	setenv_error(char ***str, int errcode)
{
	free_spl(str);
	return (errcode);
}

int			bltn_setenv(char **args, ENV *env)
{
	t_avln	*node;
	int		i;
	char	**spl;

	i = 0;
	while (args[++i])
	{
		spl = ft_strsplit(args[i], '=');
		if (!spl[1])
			spl[1] = ft_strdup("");
		if (ft_avl_search(env->locals, spl[0]) != 0)
			return (setenv_error(&spl, 242));
		else
		{
			if (!(node = ft_avl_node(spl[0], spl[1], ft_strlen(spl[1]) + 1)))
				return (setenv_error(&spl, 210));
			if (ft_avl_set(env->globals, node) == -1)
				return (setenv_error(&spl, 210));
		}
		free_spl(&spl);
	}
	return (0);
}
