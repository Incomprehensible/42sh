/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 19:14:59 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/08 15:19:01 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bltn.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"

int				cd_error_put(int mode, DSTRING *path, ENV *env)
{
	DSTRING		*buf;

	if (!(buf = dstr_new("")))
		sys_fatal_memerr("ALLOCATION FAILED CD");
	dstr_insert_str(buf, "cd: ", buf->strlen);
	dstr_insert_dstr(buf, path, buf->strlen);
	path = 0;
	if (mode == CD_MODE_ERROR_ND)
		sys_error_handler(buf->txt, E_NODIR, env);
	else if (mode == CD_MODE_ERROR_NE)
		sys_error_handler(buf->txt, E_NOFND, env);
	else if (mode == CD_MODE_ERROR_PD)
		sys_error_handler(buf->txt, E_PERMF, env);
	else
	{
		dstr_insert_str(buf, ": UNDEFINED ERROR", buf->strlen);
		sys_perror(buf->txt, 2, env);
	}
	dstr_del(&buf);
	return (2);
}

int				bltn_cd_countargs(char **args)
{
	int		i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int				bltn_cd(char **args, ENV *env)
{
	int				sttus;
	int				mode;
	DSTRING			*path;

	path = NULL;
	sttus = 0;
	mode = cd_parse_args(args, env, &path);
	if (mode < 1)
		sttus = cd_error_put(mode, path, env);
	else if (mode == CD_MODE_LOGIC)
		sttus = cd_logic(&path);
	else if (mode == CD_MODE_PHYSP)
		sttus = cd_phpath(&path);
	env_set_variable("PWD", g_pwd, env);
	env_set_variable("OLDPWD", g_oldpwd, env);
	if (path)
		dstr_del(&path);
	return (sttus);
}
