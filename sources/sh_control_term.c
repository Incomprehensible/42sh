/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_control_term.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 18:35:15 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/15 18:41:36 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include <termios.h>

size_t			sh_esc(size_t index, const size_t max)
{
	char		ch;

	ch = ft_getch();
	if (ch == '[')
	{
		ch = ft_getch();
		if (ch == LEFT[0] && index > 0)
			return (--index);
		else if (ch == RIGHT[0] && index < max)
			return (++index);
	}
	return (index);
}

int				ft_getch(void)
{
	struct		termios oldt;
	struct		termios newt;
	char		ch;

	ch = 0;
	tcgetattr(0, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSANOW, &newt);
	read(0, &ch, 1);
	tcsetattr(0, TCSANOW, &oldt);
	return ((int)ch);
}
