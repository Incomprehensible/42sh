/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 22:16:24 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/12/17 15:05:38 by hgranule         ###   ########.fr       */
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
	const int	ctrl[] = {0x1, 0x5, 0x15, 0x14, 0x2, 0x6, 0xc, \
							0x12, 0x18, 0x10, 0xb};
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
	int			apof;
	int			dqoute;

	sp = 0;
	i = -1;
	apof = 0;
	dqoute = 0;
	while (++i < buf->strlen)
	{
		if (buf->txt[i] == '\'')
			apof = !apof;
		if (buf->txt[i] == '\"')
			dqoute = !dqoute;
		if (buf->txt[i] == '(' && buf->txt[i + 1] == '(')
			i = skip_math(buf->txt, i + 2);
		if (buf->txt[i] == ' ' || buf->txt[i] == '/')
			sp = i;
		if (!apof && !dqoute && ft_memchr("*[?", buf->txt[i], 3))
			return (sp);
	}
	return (-1);
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
		else if (indch.ch == RIGHT[0] && (size_t)indch.ind < max \
				&& !(indch.select = 0) && ++indch.ind)
			return (indch);
		else if ((indch.ch == UP[0] || indch.ch == DOWN[0]) && \
				!(indch.select = 0))
			return (show_history(buf, indch));
		else if (indch.ch == 51 && !(indch.select = 0))
			sh_del_char(buf, indch.ind, ft_getch());
	}
	return (indch);
}
