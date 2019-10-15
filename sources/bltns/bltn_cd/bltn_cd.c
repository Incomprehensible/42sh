/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 04:21:28 by fnancy            #+#    #+#             */
/*   Updated: 2019/10/15 17:35:33 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include <dirent.h>

static int	bltn_cd_rmlast(t_dlist **path)
{
	t_dlist *last;
	char	*str;
	
	str = (char *)(*path)->content;
	if (ft_strequ(str, "\0"))
		return (1);
	last = (*path);
	while (last->next != NULL)
		last = last->next;
	ft_dlst_delcut(&last, free);
	return (1);
}

static void	bltn_cd_create_new_path(t_dlist **path, char *str)
{
	bltn_cd_destroy_path(&(*path));
	(*path) = ft_dlstnew(str, ft_strlen(str) + 1);
}

static void	bltn_cd_setglobals(ENV *env, DSTRING *newpath, DSTRING *oldpath)
{
	ft_avl_set(env->globals, ft_avl_node("OLDPWD", oldpath->txt, oldpath->strlen));
	ft_avl_set(env->globals, ft_avl_node("PWD", newpath->txt, newpath->strlen));
}

static int			bltn_cd_loop(char *args, ENV *env)
{
	t_dlist	*path;
	DSTRING	*newpath;
	DSTRING	*oldpath;
	
	path = NULL;
	if (bltn_cd_parser(env, &path))
	{
		oldpath = bltn_cd_pathtostr(path);
		if (ft_strequ(args, ".."))
			bltn_cd_rmlast(&path);
		else
		{
			if (args[0] != '/')
				ft_dlstpush(&path, ft_dlstnew(args, ft_strlen(args) + 1));
			else
				bltn_cd_create_new_path(&path, args);
		}
		newpath = bltn_cd_pathtostr(path);
		if (chdir(newpath->txt) == -1)
			return (0);// ОБРАБОТКА ОШИБКИ
		bltn_cd_setglobals(env, newpath, oldpath);
		dstr_del(&newpath);
		bltn_cd_destroy_path(&path);
		return (1);
	}
	return (0);	
}

int			bltn_cd(char **args, ENV *env)
{
	char	**path;

	if (!args[2])
	{
		if (ft_strequ(args[1], "/"))
			return (bltn_cd_loop(args[1], env));
		path = ft_strsplit(args[1], '/');		
		while (*path)
		{
			bltn_cd_loop(*path, env);
			path++;
		}
	}
	return (0);
}