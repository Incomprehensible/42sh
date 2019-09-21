/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 12:40:26 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/09/21 15:16:49 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include <sys/ioctl.h>
#include "sh_termcomand.h"

void				clear_screen(void)
{
	ft_putstr(CLRSCR);
	ft_putstr("\x001b[100A");
}

char				is_ctrl(const t_indch indch)
{
	if (indch.ch == 0x1 || indch.ch == 0x5 \
		|| indch.ch == 0x15 || indch.ch == 0x14 \
		|| indch.ch == 0x18 || indch.ch == 0x06 \
		|| indch.ch == 0x0e || indch.ch == 0x12)
		return (1);
	return (0);
}
