/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_rewrite_clip.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/29 22:14:05 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/29 22:34:31 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"

static DSTRING	*dstr_copy(const DSTRING *dstr, int bi, int ei)
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

static size_t	cut_lines(size_t ind, size_t to_print, struct winsize term, \
					int *lensi)
{
	if (ind == 0)
		to_print = term.ws_col - (lensi[0] % term.ws_col);
	else
		to_print = term.ws_col;
	if (lensi[2] / term.ws_col == (int)ind)
		to_print += 4;
	if (lensi[3] / term.ws_col == (int)ind)
		to_print += 4;
	return (to_print);
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
		to_print = cut_lines(ind, to_print, term, lensi);
		write(STDOUT_FILENO, b_ptr, to_print);
		ft_putstr_fd("\n", STDOUT_FILENO);
		b_ptr += to_print;
		++ind;
	}
	ft_putstr_fd(b_ptr, STDOUT_FILENO);
	dstr_del(&buf);
}
