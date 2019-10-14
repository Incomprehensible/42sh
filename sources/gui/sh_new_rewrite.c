/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_new_rewrite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 20:40:08 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/10/14 20:46:06 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "dstring.h"
#include "libft.h"
#include <unistd.h>
#include "sys_tools/sys_errors.h"

void			sh_putstr_term(const DSTRING *buf, struct winsize term, int len_p)
{
	int			ind;

	ind = -1;
	while (buf->txt[++ind])
	{
		ft_putchar(buf->txt[ind]);
		if (!((ind + len_p + 1) % term.ws_col))
			ft_putchar('\n');
	}
}

void			sh_movec_front(struct winsize term, int mov_front, int len_all, int index)
{
	int 	lines;
	int		segment;

	char	*n;
	char	*cmd_str;
	
	lines = 0;
	segment = len_all % term.ws_col;
	if ((len_all - segment) > index)
		lines = ((len_all + (term.ws_col - segment)) - index) / term.ws_col;
	if (lines)
	{
		n = ft_itoa(lines);
		cmd_str = ft_concat(3, "00", "\x001b[", n, "B");
		ft_putstr(cmd_str);
		free(n);
		free(cmd_str);
	}
}

void			sh_clear_line(struct winsize term, int len_all, int index)
{
	int		lines;

	lines = len_all / term.ws_col;
	while (lines--)
	{
		ft_putstr("\x001b[2K");
		ft_putstr(MOVEUP);
	}
	ft_putstr("\x001b[2K");
	ft_putstr(MOVEBGN);
}

void			sh_clear_buf(struct winsize term, int len_p, int index)
{
	int len_all;
	int mov_front;
	
	len_all = prebuf->strlen + len_p;
	mov_front = len_all - (index + len_p);
	if (len_all >= term.ws_col)
		sh_movec_front(term, mov_front, len_all, preind + len_p);
	sh_clear_line(term, len_all, index + len_p);
}

void			sh_new_move_cors(const DSTRING *buf, struct winsize term, int len_p, int index)
{
	int 	move_back;
	int		len_all;
	int		segment;
	int 	lines;
	char	*n;
	char	*cmd_str;

	len_all = buf->strlen + len_p;
	segment = len_all % term.ws_col;
	move_back = len_all - (index + len_p);
	if (!(lines = 0) && move_back > segment)
	{
		move_back -= segment;
		lines = (move_back / term.ws_col) + 1;
		move_back = move_back % term.ws_col;
		move_back -= 1;
		ft_putstr("\x001b[1000C");
	}
	if (lines)
	{
		n = ft_itoa(lines);
		cmd_str = ft_concat(3, "00", "\x001b[", n, "A");
		ft_putstr(cmd_str);
		free(n);
		free(cmd_str);
	}
	if (move_back)
	{
		// printf("\n%d\n", move_back);
		n = ft_itoa(move_back);
		cmd_str = ft_concat(3, "00", "\x001b[", n, "D");
		ft_putstr(cmd_str);
		free(n);
		free(cmd_str);
	}
}

void			sh_new_rewrite(const DSTRING *prompt, const DSTRING *buf,\
						const size_t index)
{
	struct winsize		term;
	int					len_p;

	len_p = ft_strlen(prompt->txt);
	ioctl(0, TIOCGWINSZ, &term);
	sh_clear_buf(term, len_p, index);
	ft_putstr(prompt->txt);
	sh_putstr_term(buf, term, len_p);
	if (buf->strlen != index)
		sh_new_move_cors(buf, term, len_p, index);
	dstr_del(&prebuf);
	prebuf = dstr_new(buf->txt);
	preind = index;
}
