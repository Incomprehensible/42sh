/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 22:16:24 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/28 17:34:01 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"

void			clear_screen(void)
{
	ft_putstr(CLRSCR);
	ft_putstr("\x001b[100A");
}

char			is_ctrl(const char ch)
{
	const int	ctrl[] = {0x1, 0x5, 0x15, 0x14, 0x2, 0x6, 0xc, 0x12};
	size_t		i;

	i = -1;
	while (++i < sizeof(ctrl) / sizeof(int))
		if (ch == ctrl[i])
			return (1);
	return (0);
}

int				is_reg(DSTRING *buf)
{
	int			i;
	int			sp;

	sp = 0;
	i = -1;
	while (++i < buf->strlen)
	{
		if (buf->txt[i] == ' ')
			sp = i;
		if (ft_memchr("*[?", buf->txt[i], 3))
			return (sp);
	}
	return (-1);
}
