/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_setenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:46:16 by fnancy            #+#    #+#             */
/*   Updated: 2019/12/05 15:13:38 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

static int	setenv_error(char ***str, int errcode)
{
	free_spl(str);
	if (errcode == 1488)
		return (sys_perror("setenv: '=' - bad input", 2, 0));
	return (errcode);
}

static void	free_spl_here(char **args)
{
	if (args[1])
		free(args[1]);
	if (args[0])
		free(args[0]);
	free(args);
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
		if (!spl[0])
			return (setenv_error(&spl, 1488));
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
		free_spl_here(spl);
	}
	return (0);
}
