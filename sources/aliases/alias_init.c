/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 14:38:07 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/07 02:10:01 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aliases.h"

static void	alias_init_free(char ***spl, char **str2)
{
	free_spl(&(*spl));
	if ((*str2))
		free(*str2);
}

int			alias_init(ENV *env)
{
	/*int		fd;
	char	*str;
	char	**spl;

	if ((fd = open(FILE_ALIASES, O_CREAT | O_RDONLY, S_IRWXU | S_IRWXG)) < 0)
		return (-1);
	spl = NULL;
	while (get_next_line(fd, &str))
	{
		spl = ft_strsplit(str, '=');
		if ((ft_avl_set(env->aliases, ft_avl_node(spl[0], (char *)spl[1],\
					ft_strlen(spl[1]) + 1)) == -1))
		{
			alias_init_free(&spl, &str);
			if ((close(fd)) != 0)
				return (-1);
			return (env_init_error(env, 12));
		}
		alias_init_free(&spl, &str);
	}
	if ((close(fd)) != 0)
		return (-1);*/
	return (1);
}
