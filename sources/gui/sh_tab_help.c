/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_tab_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 07:43:03 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/16 20:34:07 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"

void			subst_name(DSTRING **buf, t_darr overlap, \
				int ind, t_name_ind n_ind)
{
	return ;
}

int				ind_begin_cmd(DSTRING *buf)
{
	int			ind;

	if ((ind = dstrrchr(buf, ';')) != -1)
	{
		while (ft_isspace(buf->txt[ind + 1]))
			++ind;
		return (ind + 1);
	}
	ind = 0;
	while (buf->txt && ft_isspace(buf->txt[ind]))
		++ind;
	return (ind);
}

char			sh_check_back_slash(DSTRING **buf, const ssize_t start_dir)
{
	DSTRING		*tmp;

	if (start_dir == (*buf)->strlen)
		dstr_insert_ch((*buf), '.', start_dir);
	if ((*buf)->txt[(*buf)->strlen - 1] == '\\')
	{
		tmp = dstr_str_swpc((*buf), "/", (*buf)->strlen - 1);
		if (sh_isdir(tmp, start_dir))
			dstr_cutins_str(buf, "/", (*buf)->strlen - 1);
		dstr_del(&tmp);
		return (1);
	}
	if (sh_isdir((*buf), start_dir) && (*buf)->txt[(*buf)->strlen - 1] != '/')
	{
		dstr_insert_str((*buf), "/", (*buf)->strlen);
		return (1);
	}
	return (1);
}

int				sh_tab_loop_help(t_darr overlap, DSTRING **buf, \
					int fl, t_name_ind n_ind)
{
	if (fl == 0 && overlap.count > 1)
		put_col(overlap, (*buf), n_ind.indch);
	else if (overlap.count > 1 && fl != 0)
		subst_name(buf, overlap, n_ind.ind++, n_ind);
	else if (overlap.count == 1)
		subst_name(buf, overlap, 0, n_ind);
	if ((size_t)n_ind.ind == overlap.count)
		n_ind.ind = 0;
	return (n_ind.ind);
}

char			is_tab_space(DSTRING *buf)
{
	int		i;

	i = -1;
	while (buf->txt[++i])
		if (buf->txt[i] != ' ')
			return (0);
	return (1);
}

char			is_get_variable(DSTRING *buf)
{
	int		i;

	i = buf->strlen;
	while (--i > -1)
	{
		if (buf->txt[i] == '$')
			return (i);
		if (buf->txt[i] == ' ')
			return (-1);
	}
	return (-1);
}

t_darr			sh_add_variables(DSTRING **buf, ENV *env, int ind_var)
{
	// DSTRING		*var;
	// t_darr		all_var;
	t_darr		overlap;
	
	// var = dstr_slice((*buf), ind_var, (*buf)->strlen);
	// all_var = env_get_keys(env);
	// overlap = sh_cmp_darr(all_var, var);
	// dstr_del(&var);
	// free_t_darr(&all_var);
	return (overlap);
}

// t_darr			sh_add_executable_file(DSTRING **buf, ENV *env)
// {
	
// }

t_darr			sh_tab_help(DSTRING **buf, ENV *env, t_indch *indch)
{
	ssize_t		start_dir;
	t_darr		overlap;
	int			ind_var;

	overlap.count = 0;
	// if (indch.input == 2)
		// return (sh_add_executable_file(buf, env));
	if (is_tab_space((*buf)))
	{
		overlap.count = -1;
		return (overlap);
	}
	if ((ind_var = is_get_variable((*buf))) > -1)
	{
		indch->is_var = 1;
		overlap = sh_add_variables(buf, env, ind_var);
	}
	else if ((start_dir = sh_dstr_iscmd((*buf))) == -1)
		overlap = sh_add_cmd(buf, env);
	else if (sh_check_back_slash(buf, start_dir))
		overlap = sh_add_path(buf, start_dir);
	sh_new_rewrite(indch->prompt, (*buf), (*buf)->strlen);
	return (overlap);
}
