/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_control_term.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 18:35:15 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/18 20:38:44 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

struct termios	g_std_term;

t_indch			sh_esc(t_indch indch, const size_t max, DSTRING **buf, \
					ENV *envr)
{
	if (!indch.fl)
		indch.ch = ft_getch();
	if (indch.ch == '[' || indch.fl)
	{
		if (!indch.fl)
			indch.ch = ft_getch();
		indch.fl = 0;
		if (indch.ch == LEFT[0] && indch.ind > 0)
		{
			indch.ind--;
			return (indch);
		}
		else if (indch.ch == RIGHT[0] && (size_t)indch.ind < max)
		{
			indch.ind++;
			return (indch);
		}
		else if (indch.ch == UP[0] || indch.ch == DOWN[0])
			return (show_history(buf, indch, envr));
		else if (indch.ch == 51)
			sh_del_char(buf, indch.ind, ft_getch());
	}
	return (indch);
}

int				ft_getch(void)
{
	char			ch;

	ch = 0;
	if ((read(0, &ch, 1)) <= 0)
		ch = (char)0xff;
	return ((int)ch);
}

void			sys_term_init(void)
{
	tcgetattr(0, &g_std_term);
}

void			sys_term_restore(void)
{
	tcsetattr(0, TCSANOW, &g_std_term);
}

void			sys_term_noecho(void)
{
	struct termios	newt;

	newt = g_std_term;
	newt.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSANOW, &newt);
}
