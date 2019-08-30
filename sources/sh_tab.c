/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 18:36:59 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/30 08:36:13 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"

t_darr			sh_add_path(DSTRING **buf, size_t start_dir)
{
	DSTRING		*str;
	DSTRING		*path;
	t_darr		content;
	t_darr		overlap;

	path = sh_get_path((*buf), start_dir);
	content = sh_dir_content(path->txt);
	str = dstr_slice((*buf), start_dir, (*buf)->strlen);
	overlap = sh_cmp_darr(content, str);
	dstr_del(&str);
	dstr_del(&path);
	correct_namedir_buf(overlap, buf, start_dir);
	free_darr_n(content.strings, content.count);
	return (overlap);
}

t_darr			sh_add_cmd(DSTRING **buf, t_envp *env)
{
	t_darr		allcmd;
	t_darr		overlap;
	int			begin_cmd;
	DSTRING		*cmd;

	begin_cmd = ind_begin_cmd((*buf));
	cmd = dstr_slice((*buf), begin_cmd, (*buf)->strlen);
	allcmd = get_list_cmds(env);
	overlap = sh_cmp_darr(allcmd, cmd);
	correct_namedir_buf(overlap, &cmd, 0);
	dstr_cutins_dstr(buf, cmd, begin_cmd);
	free_darr_n(allcmd.strings, allcmd.count);
	dstr_del(&cmd);
	return (overlap);
}

int				get_ind_name(DSTRING *buf)
{
	int			rez;
	int			i;
	ssize_t		smcln;
	ssize_t		space;

	smcln = dstrrchr(buf, ';');
	if (smcln != -1 && (i = smcln + 1))
	{
		space = dstrrchr(buf, ' ');
		while (ft_isspace(buf->txt[i]))
			i++;
		if (i >= space + 1)
			return (i);
	}
	if ((rez = dstrrchr(buf, '/')) != -1)
		return (rez + 1);
	else if ((rez = dstrrchr(buf, ' ')) != -1)
		return (rez + 1);
	return (0);
}

t_darr			sh_tab_help(DSTRING **buf, t_envp *env)
{
	ssize_t		start_dir;
	t_darr		overlap;

	overlap.count = 0;
	if ((start_dir = sh_dstr_iscmd((*buf))) == -1)
		overlap = sh_add_cmd(buf, env);
	else if (sh_check_back_slash(buf, start_dir))
		overlap = sh_add_path(buf, start_dir);
	sh_rewrite((*buf), (*buf)->strlen);
	return (overlap);
}

t_indch			sh_tab(DSTRING **buf, size_t index, t_envp *env, t_indch indch)
{
	t_darr		overlap;
	int			fl;
	int			ind;
	int			ind_name;

	overlap = sh_tab_help(buf, env);
	fl = 0;
	ind = 1;
	ind_name = get_ind_name((*buf));
	while ((indch.ch = ft_getch()) == TAB)
	{
		if (fl++ == 0 && overlap.count > 1 && sort_darr(&overlap))
			put_col(overlap);
		if (overlap.count > 1 && fl > 1)
			subst_name(buf, overlap, ind++, ind_name);
		if (ind == overlap.count)
			ind = 0;
		if (overlap.count == 1)
			break ;
	}
	free_darr_n(overlap.strings, overlap.count);
	indch.ind = (*buf)->strlen;
	indch.fl = 0;
	return (indch);
}
