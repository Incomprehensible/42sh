/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 22:08:20 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/13 06:45:24 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int			bltn_pwd(char **args, ENV *env)
{
	char	*flgs;
	char	buf[150];
	int		i;

	flgs = ft_parse_flags(args);
	if (ft_strrchr(flgs, 'P') != NULL)
	{
		if ((i = readlink((char *)ft_avl_search(env->globals, "PWD")->content, buf,\
						sizeof(buf))) < 0)
		{
			ft_putendl((char *)ft_avl_search(env->globals, "PWD")->content);
			return (1);
		}
		buf[i] = '\0';
		ft_putendl(buf);
	}
	else
		ft_putendl((char *)ft_avl_search(env->globals, "PWD")->content);
	return (1);
}