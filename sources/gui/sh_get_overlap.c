/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_get_overlap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 15:26:44 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/19 15:56:54 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sys_tools/sys_tools.h"

int				is_all_space(DSTRING *buf, t_indch *indch)
{
	int		i;

	i = -1;
	while (buf->txt[++i])
		if (buf->txt[i] != ' ')
			return (0);
	indch->ind++;
	dstr_insert_ch(buf, ' ', buf->strlen);
	return (1);
}

t_darr			sh_get_cmd(t_buf *buffer, ENV *env)
{
	t_darr			allcmd;
	t_darr			overlap;

	if (is_executable(buffer))
		allcmd = get_executable_files(buffer->dir->txt);
	else
		allcmd = env_get_bins(env);
	overlap = sh_cmp_darr(allcmd, buffer->sub);
	free_t_darr(&allcmd);
	return (overlap);
}

t_darr			sh_get_file(DSTRING *dir, DSTRING **sub, int ind_inp, ENV *env)
{
	t_darr		content;
	t_darr		overlap;
	DSTRING		*path;

	ind_inp = 0;
	env = 0;
	if (!dir)
		path = get_path(*sub, 0);
	else
		path = get_path(dir, 1);
	if (!ft_strequ(path->txt, "error"))
		content = sh_dir_content(path->txt, (*sub));
	else
	{
		overlap.count = -1;
		return (overlap);
	}
	dell_slash(sub);
	overlap = sh_cmp_darr(content, *sub ? *sub : dir);
	free_t_darr(&content);
	dstr_del(&path);
	return (overlap);
}

t_darr			sh_get_var(t_buf *buffer, ENV *env)
{
	t_darr			allvar;
	t_darr			overlap;

	if (is_var(buffer, &allvar, env))
		return (allvar);
	if (buffer->sub->txt[0] == '{')
		allvar = env_get_keys(env, "{");
	else
		allvar = env_get_keys(env, "");
	overlap = sh_cmp_darr(allvar, buffer->sub);
	free_t_darr(&allvar);
	return (overlap);
}

t_darr			get_overlap(t_buf *buffer, t_indch *indch, ENV *env)
{
	t_darr		overlap;

	overlap.count = 0;
	if (is_all_space(buffer->buf, indch) && (overlap.count = -1))
		return (overlap);
	else if (indch->type_inp == 0)
		return (sh_get_cmd(buffer, env));
	else if (indch->type_inp == 1)
		return (sh_get_file(buffer->dir, &(buffer->sub), indch->ind_inp, env));
	else if (indch->type_inp == 2)
		return (sh_get_var(buffer, env));
	return (overlap);
}
