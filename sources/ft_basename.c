/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_basename.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 20:45:07 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/27 05:27:48 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_avl_tree.h"
#include "ft_string.h"
#include "parser.h"

char		*ft_basename(const char *path)
{
	char	*it;

	it = ft_strrchr(path, '/');
	if (*(++it) == 0)
		return (ft_strdup("/")); // MALLOC DEFENSE HIGHER
	return (ft_strdup(it));
}

int			sh_check_f_b(char *name, ENV *envr)
{
	if (ft_avl_search(envr->funcs, name))
		return (1);
	else if (ft_avl_search(envr->builtns, name))
		return (2);
	return(0);
}

char		*sh_checkpathes(const char *cmd, char **pathes)
{
	int			i;
	size_t		len[2];
	char		*str;

	i = -1;
	len[0] = ft_strlen(cmd);
	while (pathes[++i])
	{
		len[1] = ft_strlen(pathes[i]) + len[0] + 1;
		str = ft_strnew(len[1]);
		ft_strcat(str, pathes[i]);
		ft_strcat(str, "/");
		ft_strcat(str, cmd);
		if (access(str, F_OK) == 0)
			return (str);
		free(str);
	}
	return (0);
}

char		*sh_checkbins(const char *cmd, ENV *envir)
{
	t_avln		*var_node;
	char		**pathes;
	char		*str;

	if (((*cmd == '.' && *(cmd + 1) == '/') \
	|| *cmd == '/') && access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	else if (*cmd == '.' && *(cmd + 1) == '/')
		return (0);
	if (!(var_node = ft_avl_search(envir->globals, "PATH")) || \
	!(pathes = ft_strsplit(var_node->content, ':')))
		return (0);
	str = sh_checkpathes(cmd, pathes);
	if (pathes)
		et_rm_warr(pathes);
	if (str)
		return (str);
	return (0);
}
