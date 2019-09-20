/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:25:23 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/20 21:49:59 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sys_tools/sys_hidden.h"
#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"

void		prs_fatal_memerr(char *mess)
{
	char	buff[FATAL_BUFF_MSG_LEN];

	ft_bzero(buff, FATAL_BUFF_MSG_LEN);
	ft_strcpy(buff, SHELL_NAME_STD);
	ft_strcat(buff, ": FATAL ERROR: ");
	ft_strcat(buff, mess);
	ft_strcat(buff, "\n");
	ft_strcat(buff, SHELL_NAME_STD);
	ft_strcat(buff, " is CRASHED, press any button to exit!\n");
	sys_getch();
	exit(255);
}

int			prs_error_handler(int ecode, int *stat, ENV *envr, EXPRESSION *expr)
{
	DSTRING		*message;

	if (!(message = dstr_new(SHELL_NAME_STD)))
		prs_fatal_memerr("MALLOC CALL FAILED");
	dstr_insert_str(message, ": ", MAX_LL);
	dstr_insert_str(message, expr->args[0], MAX_LL);
	dstr_insert_str(message, ": ", MAX_LL);
	dstr_insert_str(message, sys_get_std_message(ecode), MAX_LL);
	sys_error_message(message->txt, message->strlen);
	dstr_del(&message);
	return (0);
}