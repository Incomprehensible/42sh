/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 14:38:07 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/10 17:03:25 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aliases.h"

static void	alias_init_free(char ***spl, DSTRING **str2)
{
	free_spl(&(*spl));
	dstr_del(&(*str2));
}

static int	alias_init_closefd(int fd, DSTRING **str)
{
	dstr_del(&(*str));
	if ((close(fd)) != 0)
		return (-1);
	return (1);
}

int			alias_init(ENV *env)
{
	int		fd;
	DSTRING	*str;
	char	**spl;
	char	*name;

	name = ft_strjoin((char *)ft_avl_search(env->globals, "HOME")->content,\
			FILE_ALIASES);
	if ((fd = open(name, O_CREAT | O_RDONLY, S_IRWXU | S_IRWXG)) < 0)
		return (-1);
	free(name);
	while (get_next_line(fd, &str))
	{
		spl = ft_strsplit(str->txt, '=');
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
	return (alias_init_closefd(fd, &str));
}
