/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:33:01 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/22 16:13:17 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys_tools/sys_errors.h"
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
