/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 18:36:59 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/17 19:30:23 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

t_darr			sh_add_path(DSTRING **buf, size_t start_dir)
{
	DSTRING		*str;
	DSTRING		*path;
	t_darr		darr;
	t_darr		rez;
	
	path = sh_get_path((*buf), start_dir);
	darr = sh_dir_content(path->txt);
	
	str = dstr_slice((*buf), start_dir, (*buf)->strlen);
	rez = sh_cmp_darr(darr, str);
	free_darr(darr.strings);
	dstr_del(&str);
	dstr_del(&path);
	correct_namedir_buf(rez, buf, start_dir);
	return (rez);
}

t_darr			sh_add_cmd(DSTRING **buf, t_envp *env)
{
	t_darr		allcmd;
	t_darr		cmd;

	allcmd = get_list_cmds(env);
	cmd = sh_cmp_darr(allcmd, (*buf));
	correct_namedir_buf(cmd, buf, 0);
	return (cmd);
}

char			sh_check_back_slash(DSTRING **buf, const ssize_t start_dir)
{
	if ((*buf)->txt[(*buf)->strlen - 1] == '\\' && sh_isdir((*buf), start_dir))
	{
		dstr_cutins_str(buf, "/", (*buf)->strlen - 1);
		return (0);
	}
	if (sh_isdir((*buf), start_dir) && (*buf)->txt[(*buf)->strlen-1] != '/')
	{
		dstr_insert_str((*buf), "/", (*buf)->strlen);
		return (0);
	}
	return (1);
}

size_t			sh_tab(DSTRING **buf, size_t index, t_envp *env)
{
	ssize_t		start_dir;
	t_darr		darr;
	
	darr.count = 0;
	if ((start_dir = sh_dstr_iscmd((*buf))) == -1)
		darr = sh_add_cmd(buf, env);
	else if (sh_check_back_slash(buf, start_dir))
		darr = sh_add_path(buf, start_dir);
	if (darr.count > 1)
		put_col(darr);
	// printf("\n%zu\n", darr.count);
	// free_darr(darr.strings);
	return ((*buf)->strlen);
}