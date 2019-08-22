/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_term.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 14:14:42 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/22 18:04:12 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"
#include "dstring.h"
#include "libft.h"

void			sh_move_cursor(const size_t n, const char *side)
{
	char		*move_cors_r;
	char		*move;
	
	if (n > 0)
	{
		move = ft_itoa(n);
		move_cors_r = ft_concat(3, "00", "\x1b[", move, side);
		ft_putstr(move_cors_r);
		free(move_cors_r);
		free(move);
	}
}

void			sh_move_del(const size_t n, const char *side)
{
	sh_move_cursor(n, side);
	ft_putstr(DELCHR);
}

void			sh_rewrite(const DSTRING *buf, const size_t index)
{
	char		*move_cursor_begin;
	
	move_cursor_begin = ft_concat(3, "00", "\x1b[", "1000", LEFT);
	ft_putstr(move_cursor_begin);
	ft_putstr(CLEARL);
	ft_putstr(buf->txt);
	ft_putstr(move_cursor_begin);
	free(move_cursor_begin);
	sh_move_cursor(index, RIGHT);
}

ssize_t			sh_del_char(DSTRING **buf, size_t index, const char flag)
{
	if (flag == BAKSP && index > 0)
	{
		sh_move_del(1, LEFT);
		dstr_del_char(buf, --index);
		return (index);
	}
	else
		dstr_del_char(buf, index);
	return (index);
}
