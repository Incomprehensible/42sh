/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:25:23 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/22 11:00:45 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"

int			prs_error_handler(int ecode, int *stat, ENV *envr, EXPRESSION *expr)
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
	dstr_insert_str(message, expr->args[0], MAX_LL);
	dstr_insert_str(message, ": ", MAX_LL);
	dstr_insert_str(message, sys_get_std_message(ecode), MAX_LL);
	sys_error_message(message->txt, message->strlen);
	dstr_del(&message);
	return (0);
}