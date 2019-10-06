/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:37:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/02 05:17:56 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_tools.h"
#include "ft_mem.h"
#include "ft_string.h"
#include "stdlib.h"
#include "dstring.h"
#include "env.h"

char		*errmess[ERR_CNT] =
{
	"",
	"Command not found",
	"Fork call failed",
	"Exec call failed",
	"Pipe call failed",
	"Permissions denied",
	"Redirection denied",
	"Bad file descriptor",
	"Memory allocation failed",
	"Is a directory",
	"Is not a regular file",
	"File creation failed",
	"File open failed, doesn't exist",
	"File open failed, permissions denied",
	"File open failed"
	
};

char			*sys_get_std_message(int ecode)
{
	if (ecode > ERR_CNT)
		ecode = 0;
	return (errmess[ecode]);
}

void			sys_fatal_memerr(char *mess)
{
	char	buff[FATAL_BUFF_MSG_LEN];

	ft_bzero(buff, FATAL_BUFF_MSG_LEN);
	ft_strcpy(buff, SHELL_NAME_STD);
	ft_strcat(buff, ": Fatal error is occured: ");
	ft_strcat(buff, mess);
	ft_strcat(buff, "\n");
	ft_strcat(buff, "In this scope is, press any button to exit.");
	sys_error_message(buff, 0);
	sys_getch();
	exit(255);
}

int		sys_error_handler(char *descr, int ecode, ENV *envr)
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
	if (descr)
	{
		dstr_insert_str(message, descr, MAX_LL);
		ecode ? dstr_insert_str(message, ": ", MAX_LL) : 0;
	}
	ecode ? dstr_insert_str(message, sys_get_std_message(ecode), MAX_LL) : 0;
	sys_error_message(message->txt, message->strlen);
	dstr_del(&message);
	return (-ecode);
}
