/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 04:21:28 by fnancy            #+#    #+#             */
/*   Updated: 2019/11/13 16:16:55 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include <dirent.h>
#include "sys_tools/sys_tools.h"
#include "sys_tools/sys_errors.h"

static int	bltn_cd_rmlast(t_dlist **path)
{
	t_dlist *last;
	char	*str;
	int		counter;

	counter = 0;
	str = (char *)(*path)->content;
	if (ft_strequ(str, "\0"))
		return (1);
	last = (*path);
	while (last->next != NULL)
	{
		last = last->next;
		counter++;
	}
	if (counter == 0)
		ft_dlst_delcut(path, et_rm_str);
	else
		ft_dlst_delcut(&last, et_rm_str);
	return (1);
}

static void	bltn_cd_create_new_path(t_dlist **path, char *str)
{
	bltn_cd_destroy_path(&(*path));
	(*path) = ft_dlstnew(str, ft_strlen(str) + 1);
}

static int	bltn_cd_setglobals(ENV *env, DSTRING **newpath,\
								DSTRING **oldpath, t_dlist **path)
{
	ft_avl_set(env->globals,\
				ft_avl_node("OLDPWD", (*oldpath)->txt, (*oldpath)->strlen + 1));
	ft_avl_set(env->globals,\
				ft_avl_node("PWD", (*newpath)->txt, (*newpath)->strlen + 1));
	dstr_del(&pwd);
	dstr_del(&oldpwd);
	pwd = dstr_new((*newpath)->txt);
	oldpwd = dstr_new((*oldpath)->txt);
	bltn_cd_freepaths(oldpath, newpath, path);
	return (0);
}

static int	bltn_cd_loop(char *args, ENV *env)
{
	t_dlist	*path;
	DSTRING	*newpath;
	DSTRING	*oldpath;

	path = NULL;
	if (ft_strequ(args, "."))
		return (0);
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
		if (chdir((char *)newpath->txt) == -1)
			return (bltn_cd_error(&oldpath, &newpath, &path));
		return (bltn_cd_setglobals(env, &newpath, &oldpath, &path));
	}
	return (1);
}

int			bltn_cd(char **args, ENV *env)
{
	char	**path;
	int		status;
	int		i;

	i = -1;
	status = 0;
	if (bltn_cd_countargs(args) < 3)
	{
		if (ft_strequ(args[1], "/"))
			return (bltn_cd_loop(args[1], env));
		path = ft_strsplit(args[1], '/');
		if (args[1][0] == '/')
			bltn_cd_concat(&path[0]);
		while (path[++i])
		{
			status = bltn_cd_loop(path[i], env);
			free(path[i]);
			if (status != 0)
				break ;
		}
		free(path);
	}
	else
		sys_error_handler("cd", E_TMARG, 0);
	return (status);
}
