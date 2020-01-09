/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:25:23 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/19 09:56:08 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"

int		prs_error_handler(int ecode, size_t type, ENV *envr, EXPRESSION *expr)
{
	t_dyn_string	*mes;

	if (!(mes = env_get_variable(SH_VAR_SHNAME, envr)) || mes->strlen == 0)
	{
		if (mes)
			dstr_del(&mes);
		if (!(mes = dstr_new(SHELL_NAME_STD)))
			sys_fatal_memerr("MALLOC CALL FAILED");
	}
	dstr_insert_str(mes, ": ", MAX_LL);
	if (type == ET_EXPR)
		dstr_insert_str(mes, expr->args[0], MAX_LL);
	else if (type == ET_SUBSH)
	{
		dstr_insert_str(mes, "(  )", mes->strlen);
		dstr_insert_str(mes, (char *)\
		((SUBSH *)expr)->commands, mes->strlen - 2);
	}
	else if (type == ET_BCKGR)
		dstr_insert_str(mes, "Background sequence", mes->strlen);
	dstr_insert_str(mes, ": ", MAX_LL);
	dstr_insert_str(mes, sys_get_std_message(ecode), MAX_LL);
	sys_error_message(mes->txt, mes->strlen);
	dstr_del(&mes);
	return (0);
}
