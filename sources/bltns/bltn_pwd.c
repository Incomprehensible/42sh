/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 22:08:20 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/19 15:25:22 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"

int			bltn_pwd(char **args, ENV *env)
{
	char	*flgs;
	char	buf[150];
	int		i;

	env = 0;
	flgs = ft_parse_flags(args);
	if (flgs && ft_strrchr(flgs, 'P') != NULL)
	{
		if ((i = readlink(g_pwd->txt,\
							buf, sizeof(buf))) < 0)
		{
			ft_putendl(g_pwd->txt);
			return (1);
		}
		buf[i] = '\0';
		ft_putendl(buf);
	}
	else
		ft_putendl(g_pwd->txt);
	return (1);
}
