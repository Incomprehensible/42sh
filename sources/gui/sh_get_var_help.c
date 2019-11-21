/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_get_var_help.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 15:38:10 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/19 16:23:16 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sys_tools/sys_tools.h"

int			is_var(t_buf *buffer, t_darr *allvar, ENV *env)
{
	t_dyn_string	*value;
	t_dyn_string	*tmp;
	int				sl;

	if ((sl = dstrrchr(buffer->sub, '/')) == -1)
		return (0);
	tmp = dstr_scerr(&(buffer->sub), 0, sl);
	dstr_insert_dstr(buffer->end, buffer->sub, 0);
	value = env_get_variable(tmp->txt, env);
	if (!value->strlen)
		return (0);
	allvar->strings[0] = value;
	allvar->count = 1;
	dstr_del_char(&(buffer->begin), buffer->begin->strlen - 1);
	dstr_del(&tmp);
	return (1);
}
