/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 15:17:40 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/01 16:34:30 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aliases.h"
#include <sys/stat.h>
#include "rms.h"

static int	alias_add_error(char ***mas)
{
	et_rm_warr((*mas));
	return (-1);
}

static int	alias_add_write(int fd, char *str)
{
	ft_putstr_fd(str, fd);
	ft_putchar_fd('\n', fd);
	return (1);
}

int			alias_add(ENV *env)
{
	int		fd;
	char	**mas;
	int		i;
	char	*name;

	name = ft_strjoin((char *)ft_avl_search(env->globals, "HOME")->content,\
			FILE_ALIASES);
	if (!(mas = ft_avl_tree_to_warr(env->aliases, &avln_todstring_key_val)))
		return (-1);
	if ((fd = open(name, O_TRUNC | O_WRONLY, S_IRWXU | S_IRWXG)) < 0)
		return (alias_add_error(&mas));
	free(name);
	i = -1;
	while (mas[++i])
	{
		if (alias_add_write(fd, mas[i]) == -1)
			return (alias_add_error(&mas));
	}
	et_rm_warr(mas);
	if ((close(fd)) != 0)
		return (-1);
	return (1);
}
