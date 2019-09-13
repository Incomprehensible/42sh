/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flagsparser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 22:09:36 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/11 22:35:44 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_parse_flags(char **argc)
{
	int		i;
	int		j;
	int		k;
	char	*res;

	i = 0;
	k = -1;
	if (!argc[1])
		return (NULL);
	if (!(res = (char *)malloc(sizeof(char) * 63)))
		return (NULL);
	while (argc[++i])
	{
		j = 0;
		if (argc[i][j] != '-')
			break ;
		while (argc[i][++j] && argc[i][j] != '-')
			if (!(ft_strrchr(res, argc[i][j])))
				res[++k] = argc[i][j];
	}
	res[++k] = '\0';
	return (res);
}
