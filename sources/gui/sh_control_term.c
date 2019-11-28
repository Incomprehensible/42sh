/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_control_term.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 18:35:15 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/28 20:58:05 by hgranule         ###   ########.fr       */
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

	i = 0;
	while (str[i])
	{
		if (str[i] != ft_getch())
			return (0);
		++i;
	}
	return (i);
}

void			clip_mode_unlish(t_indch *indch, t_clipbrd *clip, size_t max)
{
	if (!indch->select)
		clip->ind = indch->ind;
	if (indch->ch == LEFT[0] && indch->ind > 0)
		indch->ind--;
	else if (indch->ch == RIGHT[0] && (size_t)indch->ind < max)
		indch->ind++;
	indch->select = 1;
}

t_indch			sh_esc(t_indch indch, const size_t max, \
DSTRING **buf, t_clipbrd *clip)
{
	if (!indch.fl)
		indch.ch = ft_getch();
	if (indch.ch == '[' || indch.fl)
	{
		if (!indch.fl)
			indch.ch = ft_getch();
		indch.fl = 0;
		if (indch.ch == '1' && skip_str(";2") && (indch.ch = ft_getch()))
			clip_mode_unlish(&indch, clip, max);
		else if (indch.ch == LEFT[0] && indch.ind > 0 && !(indch.select = 0))
		{
			indch.ind--;
			return (indch);
		}
		else if (indch.ch == RIGHT[0] && (size_t)indch.ind < max && !(indch.select = 0))
		{
			indch.ind++;
			return (indch);
		}
		else if ((indch.ch == UP[0] || indch.ch == DOWN[0]) && !(indch.select = 0))
			return (show_history(buf, indch));
		else if (indch.ch == 51 && !(indch.select = 0))
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
