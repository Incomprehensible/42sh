/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltn_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 16:55:15 by fnancy            #+#    #+#             */
/*   Updated: 2019/09/11 17:18:51 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include <dirent.h>

static int	bltn_cd_checkpwd(char *args)
{
	if (ft_strcmp(args, ".") == 0 || ft_strcmp(args, "..") == 0)
		return (0);
	if (args[0] != '/')
		return (1);
	return (0);
}

static int	bltn_cd_err(DSTRING **str, DSTRING **str2, int r)
{
	dstr_del(&(*str));
	dstr_del(&(*str2));
	return (r);
}

static void	bltn_cd_pwdsub(DSTRING **pwd, DSTRING *name)
{
	DSTRING	*tmp;

	if (ft_strcmp(name->txt, ".") == 0)
		return ;
	if (ft_strcmp((*pwd)->txt, "/") == 0 && ft_strcmp(name->txt, "..") == 0)
		return ;
	if (ft_strcmp(name->txt, "..") == 0)
	{
		tmp = dstr_new((*pwd)->txt);
		dstr_del(&(*pwd));
		(*pwd) = dstr_slice(tmp, 0, ft_strrchr(tmp->txt, '/') - tmp->txt);
		dstr_del(&tmp);
	}
}

int			bltn_cd(char **args, ENV *env)
{
	DSTRING	*name;
	DSTRING	*pwd;

	pwd = dstr_new((char *)ft_avl_search(env->globals, "PWD")->content);
	if (!args[1])
		name = dstr_new((char *)ft_avl_search(env->globals, "HOME")->content);
	else
		name = dstr_new(args[1]);
	if (chdir(name->txt) == -1)
		return (bltn_cd_err(&name, &pwd, -1));
	if ((ft_avl_set(env->globals, ft_avl_node("OLDPWD", (char *)pwd->txt, pwd->strlen))) == -1)
		return (bltn_cd_err(&name, &pwd, -1));
	if (bltn_cd_checkpwd(name->txt))
	{
		dstr_insert_ch(pwd, '/', pwd->strlen);
		dstr_insert_str(pwd, name->txt, pwd->strlen);
	}
	else
		bltn_cd_pwdsub(&pwd, name);
	if ((ft_avl_set(env->globals, ft_avl_node("PWD", (char *)pwd->txt, pwd->strlen))) == -1)
		return (bltn_cd_err(&name, &pwd, -1));
	return (bltn_cd_err(&pwd, &name, 1));
}
