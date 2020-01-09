/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_rewrite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 20:40:08 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/29 22:19:20 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "dstring.h"
#include "libft.h"
#include <unistd.h>
#include "sys_tools/sys_errors.h"

void			sh_putstr_term(const DSTRING *buf, struct winsize term, \
				int *lensi)
{
	size_t		ind;
	size_t		lines;
	size_t		to_print;
	char		*b_ptr;

	ind = 0;
	lines = (lensi[1] + (lensi[0] % term.ws_col)) / term.ws_col;
	b_ptr = buf->txt;
	while (ind < lines)
	{
		if (ind == 0)
			to_print = term.ws_col - (lensi[0] % term.ws_col);
		else
			to_print = term.ws_col;
		write(STDOUT_FILENO, b_ptr, to_print);
		ft_putstr_fd("\n", STDOUT_FILENO);
		b_ptr += to_print;
		++ind;
	}
	ft_putstr_fd(b_ptr, STDOUT_FILENO);
}

void			sh_new_move_cors(int len_b, struct winsize term, \
					int len_p, int index)
{
	int		move_back;
	int		len_all;
	int		segment;
	int		lines;

	lines = 0;
	len_all = len_b + len_p;
	segment = len_all % term.ws_col;
	move_back = len_all - (index + len_p);
	if (move_back > segment)
	{
		move_back -= segment;
		lines = (move_back / term.ws_col) + 1;
		move_back = move_back % term.ws_col;
		move_back -= 1;
		ft_putstr("\x001b[1000C");
	}
	if (lines)
		sh_move_up_lines(lines);
	if (move_back)
		sh_move_back(move_back);
}

int				ft_color_strlen(char *str, struct winsize term)
{
	int		rez;
	int		i;
	int		fl;

	i = -1;
	fl = 0;
	rez = 0;
	while (str[++i])
	{
		while (str[i] == '\033')
			while (str[i++] != 'm')
				;
		if (str[i] == '\n')
		{
			rez += term.ws_col - (rez % term.ws_col);
			continue ;
		}
		if (!str[i])
			break ;
		++rez;
	}
	return (rez);
}

void			get_coord_caret(int *x, int *y)
{
	char			buf[64];
	int				i;
	int				c;

	write(1, "\033[6n", 4);
	ft_bzero(buf, 64);
	i = -1;
	c = 0;
	while (read(0, &(buf[++i]), 1) && buf[i] != 'R')
		if (buf[i] == ';')
			c = i + 1;
	buf[i] = '\0';
	*x = ft_atoi(buf + 2);
	*y = ft_atoi(buf + c);
}

void			sh_rewrite(const DSTRING *prompt, const DSTRING *buf,\
						const size_t index, int cli)
{
	struct winsize		term;
	int					len_p;
	int					len_b;
	int					lensi[4];

	ioctl(0, TIOCGWINSZ, &term);
	len_p = ft_color_strlen(prompt->txt, term);
	len_b = buf->strlen;
	sh_clear_buf(term, len_p, index);
	ft_putstr_fd(prompt->txt, STDOUT_FILENO);
	lensi[0] = len_p;
	lensi[1] = len_b;
	lensi[2] = (int)index;
	lensi[3] = cli;
	if (lensi[3] >= 0)
		sh_putstr_term_c((DSTRING *)buf, term, lensi);
	else
		sh_putstr_term(buf, term, lensi);
	if (len_b != (ssize_t)index)
		sh_new_move_cors(len_b, term, len_p, index);
	g_prebuf = len_b;
	g_preind = index;
}
