/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/13 21:52:46 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/14 22:23:05 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termcap.h>
#include <termios.h>
#include "libft.h"
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "dstring.h"


// справочники
// http://www.codenet.ru/progr/cpp/7/5.php
// http://rus-linux.net/MyLDP/BOOKS/lpg-04/node8.htm

int				ft_getch(void)
{
	struct		termios oldt, newt;
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

ssize_t			dstr_del_char(DSTRING **src, ssize_t n)
{
	DSTRING *begin;
	DSTRING *end;

	begin = dstr_slice((*src), 0, n);
	end = dstr_slice((*src), n + 1, (*src)->strlen);
	dstr_insert_dstr(begin, end, begin->strlen);
	dstr_del(src);
	dstr_del(&end);
	(*src) = begin;
	return (n - 1);
}
void			move_cursor(ssize_t n, char *side)
{
	char	*move_cors_r;
	
	if (n > 0)
	{
		move_cors_r = ft_concat(3, "00", "\x001b[", ft_itoa(n), side);
		ft_putstr(move_cors_r);
		free(move_cors_r);
	}
}

// "\x1b[P"
void			move_del(size_t n, char *side)
{
	move_cursor(n, side);
	ft_putstr("\x001b[P");
}

void			backspace(DSTRING	**buf, size_t index)
{
	DSTRING	*tmp;

	move_del(1, LEFT);
	dstr_del_char(buf, index);
}

char			*sh_readline(void)
{
	DSTRING	*buf;
	
	char	a[2];
	int		index;
	char	*move;

	buf = dstr_new(0);
	index = 0;
	while (1)
	{
		a[0] = ft_getch();
		// printf("0x%hhX == [%c]\n", a, (a < ' ' || a > '~') ? '$' : a);
		if (a[0] == '\n') /*CTRL-C*/
			return (buf->txt);
		else if (a[0] == 127)
		{
			index--;
			backspace(&buf, index);
		}
		else if (32 <= a[0] && a[0] <= 126) /* putchar */
		{
			dstr_insert_str(buf, a, index++);
		}
		else if (a[0] == 27)  /* ESC */
		{
			char next1 = ft_getch();
			if (next1 == 91) /* [ */
			{
				char next2 = ft_getch();
				if (next2 == 68) /* left */
					index--;
				else if (next2 == 67) /* right */
					index++;
			}
		}
		ft_putstr("\x1b[1000D"); /*back*/
		ft_putstr("\x001b[0K"); /* clear line*/
		ft_putstr(buf->txt);
		ft_putstr("\x1b[1000D"); /*back*/
		move_cursor(index, RIGHT);
	}	
	return (buf->txt);
}