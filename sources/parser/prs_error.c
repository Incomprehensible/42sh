/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:25:23 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/23 17:07:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"

int			prs_error_handler(int ecode, size_t type, ENV *envr, EXPRESSION *expr)
{
	DSTRING		*message;

	if (!(message = env_get_variable(SH_VAR_SHNAME, envr)) || message->strlen == 0)
	{
		if (message)
			dstr_del(&message);
		if (!(message = dstr_new(SHELL_NAME_STD)))
			sys_fatal_memerr("MALLOC CALL FAILED");
	}
	dstr_insert_str(message, ": ", MAX_LL);
	if (type == ET_EXPR)
		dstr_insert_str(message, expr->args[0], MAX_LL);
	else if (type == ET_SUBSH)
	{
		dstr_insert_str(message, "(  )", message->strlen);
		dstr_insert_str(message, (char *)((SUBSH *)expr)->commands, message->strlen - 2);
	}
	else if (type == ET_BCKGR)
	{
		dstr_insert_str(message, "Background sequence", message->strlen);
	}
	dstr_insert_str(message, ": ", MAX_LL);
	dstr_insert_str(message, sys_get_std_message(ecode), MAX_LL);
	sys_error_message(message->txt, message->strlen);
	dstr_del(&message);
	return (0);
}