/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:33:01 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/15 08:02:03 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_tools.h"
#include "ft_io.h"
#include "dstring.h"
#include <unistd.h>
#include <termios.h>

int		sys_error_message(char *message, size_t len)
{
	if (len == 0)
		len = ft_strlen(message);
	write(2, message, len);
	write(2, "\n", 1);
	return (0);
}

int				sys_perror(char *contex, int ret_code, ENV *envr)
{
	DSTRING		*message;

	if (!envr)
	{
		if (!(message = dstr_new(SHELL_NAME_STD)))
			sys_fatal_memerr("MALLOC CALL FAILED");
	}
	else if (!(message = env_get_variable(SH_VAR_SHNAME, envr)) \
	|| message->strlen == 0)
	{
		if (message)
			dstr_del(&message);
		if (!(message = dstr_new(SHELL_NAME_STD)))
			sys_fatal_memerr("MALLOC CALL FAILED");
	}
	if (!contex)
		contex = "Undefined error";
	dstr_insert_str(message, ": ", MAX_LL);
	dstr_insert_str(message, contex, MAX_LL);
	sys_error_message(message->txt, message->strlen);
	dstr_del(&message);
	return (ret_code);
}

int				sys_put_prompt(ENV *envr, char type)
{
	DSTRING		*prmt;

	prmt = sys_get_prompt_num(envr, type);
	write(1, prmt->txt, prmt->strlen);
	dstr_del(&prmt);
	return (0);
}

int				sys_getch(void)
{
	struct termios	oldt;
	struct termios	newt;
	char			ch;

	ch = 0;
	tcgetattr(0, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSANOW, &newt);
	if ((read(0, &ch, 1)) <= 0)
		ch = (char)0xff;
	tcsetattr(0, TCSANOW, &oldt);
	return ((int)ch);
}
