/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 22:16:24 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/18 22:18:16 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh_readline.h>

void			clear_screen(void)
{
	ft_putstr(CLRSCR);
	ft_putstr("\x001b[100A");
}

char				is_ctrl(const char ch)
{
	if (ch == 0x1 || ch == 0x5 \
		|| ch == 0x15 || ch == 0x14 \
		|| ch == 0x18 || ch == 0x06 \
		|| ch == 0x0e || ch == 0x12)
		return (1);
	return (0);
}