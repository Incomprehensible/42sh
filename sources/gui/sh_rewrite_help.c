/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_rewrite_help.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 22:06:42 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/19 15:58:39 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "dstring.h"
#include "libft.h"
#include <unistd.h>
#include "sys_tools/sys_errors.h"

void			sh_move_up_lines(int lines)
{
	char	*n;
	char	*cmd_str;

	n = ft_itoa(lines);
	cmd_str = ft_concat(3, "00", "\x001b[", n, "A");
	ft_putstr(cmd_str);
	free(n);
	free(cmd_str);
}

void			sh_move_back(int move_back)
{
	char	*n;
	char	*cmd_str;

	n = ft_itoa(move_back);
	cmd_str = ft_concat(3, "00", "\x001b[", n, "D");
	ft_putstr(cmd_str);
	free(n);
	free(cmd_str);
}

void			sh_movec_front(struct winsize term, int len_all, int index)
{
	int		lines;
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

void			sh_clear_line(struct winsize term, int len_all)
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

	len_all = g_prebuf + len_p;
	mov_front = len_all - (index + len_p);
	if (len_all >= term.ws_col)
		sh_movec_front(term, len_all, g_preind + len_p);
	sh_clear_line(term, len_all);
}
