/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_control_term.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 18:35:15 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/12/01 17:39:22 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

struct termios	g_std_term;

int				skip_str(char *str)
{
	int			i;

	i = -1;
	while (str[++i])
		if (str[i] != ft_getch())
			return (0);
	return (i);
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
