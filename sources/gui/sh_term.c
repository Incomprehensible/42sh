/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_term.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 14:14:42 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/16 18:54:26 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "dstring.h"
#include "libft.h"
#include <unistd.h>
#include "sys_tools/sys_errors.h"

void			sh_move_cursor(size_t n, const char *side, \
						const unsigned short col)
{
	char		*move_cors_r;
	char		*move;

	if (n > col && col != 0)
	{
		if (ft_strcmp(side, "D") == 0)
			move = ft_concat(3, "00", "\x001b[", ft_itoa((n - 1) / col), "F"); // LEAK
		else
			move = ft_concat(3, "00", "\x001b[", ft_itoa(n / col), "E"); // LEAK
		if (move == NULL)
			sys_fatal_memerr(sys_get_std_message(8));
		ft_putstr(move);
		n = n % col;
		free(move);
	}
	if (!ft_strcmp(side, "C") && !(n % col) && col != 0 && n != 0) // перенос строки делается с \n
	{
		ft_putstr("\n");
		ft_putstr("\x001b[0B");
		n = n % col;
	}
	if (n > 0)
	{
		if (ft_strcmp(side, "C") == 0 && n == col)
			move = ft_itoa(n + 1);
		else
			move = ft_itoa(n);
		move_cors_r = ft_concat(3, "00", "\x1b[", move, side);
		ft_putstr(move_cors_r);
		free(move_cors_r);
		free(move);
	}
}

void			sh_move_del(const size_t n, const char *side)
{
	sh_move_cursor(n, side, 0);
	ft_putstr(DELCHR);
}

size_t			clear_long_line(const DSTRING *buf, struct winsize term,\
							const size_t index, int len_n)
{
	int					lines;

	lines = buf->strlen + len_n / term.ws_col;
	sh_move_cursor(buf->strlen + len_n, LEFT, term.ws_col);
	ft_putstr(CLEARL);
	ft_putstr(NAME);
	ft_putstr(buf->txt);
	sh_move_cursor(buf->strlen + len_n, LEFT, term.ws_col);
	sh_move_cursor(index + len_n, RIGHT, term.ws_col);
	return (lines * term.ws_col);
}

ssize_t			sh_del_char(DSTRING **buf, size_t index, const char flag)
{
	if (flag == BAKSP && index > 0)
	{
		sh_move_del(1, LEFT);
		dstr_del_char(buf, --index);
		return (index);
	}
	else if (flag == DEL)
		dstr_del_char(buf, index);
	return (index);
}
