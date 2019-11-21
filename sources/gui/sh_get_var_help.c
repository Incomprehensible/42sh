/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_get_var_help.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 15:38:10 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/20 19:32:49 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sys_tools/sys_tools.h"

int			is_var(t_buf *buffer, t_darr *allvar, ENV *env)
{
	t_dyn_string	*value;
	int				ex;

	value = env_get_variable(buffer->sub->txt, env);
	if ((ex = sys_is_ex_bin(value->txt)) \
	&& (ex != -3 && ex != -2))
	{
		dstr_del(&value);
		return (0);
	}
	if (ex == -2)
		dstr_insert_ch(value, '/', value->strlen);
	check_spec_symbols(&value);
	allvar->strings[0] = value;
	allvar->count = 1;
	dstr_del_char(&(buffer->begin), buffer->begin->strlen - 1);
	return (1);
}
