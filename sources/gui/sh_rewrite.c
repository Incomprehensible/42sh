/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_rewrite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 20:40:08 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/28 20:55:05 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include "sh_readline.h"
#include "sh_termcomand.h"
#include "dstring.h"
#include "libft.h"
#include <unistd.h>
#include "sys_tools/sys_errors.h"

DSTRING			*dstr_copy(const DSTRING *dstr, int bi, int ei)
{
	DSTRING		*dstr_clone;

	dstr_clone = dstr_new(0);
	dstr_clone->strlen = dstr->strlen + 8;
	dstr_clone->bks = ((dstr_clone->strlen + 1) / DSTR_BLK_SZ) + 1;
	dstr_clone->txt = malloc(dstr_clone->bks * DSTR_BLK_SZ);
	ft_memcpy(dstr_clone->txt, dstr->txt, bi);
	ft_memcpy(dstr_clone->txt + bi, "\033[7m", 4);
	ft_memcpy(dstr_clone->txt + bi + 4, dstr->txt + bi, ei - bi);
	ft_memcpy(dstr_clone->txt + ei + 4, "\033[0m", 4);
	ft_memcpy(dstr_clone->txt + ei + 8, dstr->txt + ei, dstr->strlen - ei);
	dstr_clone->txt[dstr->strlen + 8] = '\0';
	return (dstr_clone);
}

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

void			sh_putstr_term_c(DSTRING *buf, struct winsize term, \
					int *lensi)
{
	size_t		ind;
	size_t		lines;
	size_t		to_print;
	char		*b_ptr;

	ind = 0;
	clip_index(lensi[2], lensi[3], &lensi[2], &lensi[3]);
	lines = (lensi[1] + (lensi[0] % term.ws_col)) / term.ws_col;
	buf = dstr_copy(buf, lensi[2], lensi[3]);
	b_ptr = buf->txt;
	while (ind < lines)
	{
		if (ind == 0)
			to_print = term.ws_col - (lensi[0] % term.ws_col);
		else
			to_print = term.ws_col;
		if (lensi[2] / term.ws_col == (int)ind)
			to_print += 4;
		if (lensi[3] / term.ws_col == (int)ind)
			to_print += 4;
		write(STDOUT_FILENO, b_ptr, to_print);
		ft_putstr_fd("\n", STDOUT_FILENO);
		b_ptr += to_print;
		++ind;
	}
	ft_putstr_fd(b_ptr, STDOUT_FILENO);
	dstr_del(&buf);
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

/*
** void			get_coord_caret(t_caret *caret)
** {
** 	char			buf[64];
** 	int				i;
** 	int				c;
**
** 	write(1, "\033[6n", 4);
** 	ft_bzero(buf, 64);
** 	i = -1;
** 	c = 0;
** 	while (read(0, &(buf[++i]), 1) && buf[i] != 'R')
** 		if (buf[i] == ';')
** 			c = i + 1;
** 	buf[i] = '\0';
** 	caret->y = ft_atoi(buf + 2);
** 	caret->s_y = ft_itoa(caret->y);
** 	caret->x = ft_atoi(buf + c);
** 	caret->s_x = ft_itoa(caret->x);
** }
*/

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
