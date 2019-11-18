/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_rewrite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 20:40:08 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/19 02:23:12 by hgranule         ###   ########.fr       */
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
					int len_p)
{
	size_t		ind;
	size_t		lines;
	size_t		to_print;
	char		*b_ptr;

	ind = 0;
	lines = (buf->strlen + (len_p % term.ws_col)) / term.ws_col;
	b_ptr = buf->txt;
	while (ind < lines)
	{
		if (ind == 0)
			to_print = term.ws_col - (len_p % term.ws_col);
		else
			to_print = term.ws_col;
		write(STDOUT_FILENO, b_ptr, to_print);
		ft_putstr_fd("\n", STDOUT_FILENO);
		b_ptr += to_print;
		++ind;
	}
	ft_putstr_fd(b_ptr, STDOUT_FILENO);
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

void			sh_new_move_cors(const DSTRING *buf, struct winsize term, \
					int len_p, int index)
{
	int		move_back;
	int		len_all;
	int		segment;
	int		lines;

	lines = 0;
	len_all = buf->strlen + len_p;
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

int				skip_num(char *str)
{
	int num;

	if (!str)
		return (0);
	num = ft_atoi(str);
	if (num > 100)
		return (3);
	if (num > 10)
		return (2);
	return (1);
}

int				ft_color_strlen(char *str, struct winsize term)
{
	int		rez;
	int		i;
	int 	fl;

	i = -1;
	fl = 0;
	rez = 0;
	while (str[++i])
	{
		if (str[i] == '\033')
			while (str[i] && !ft_isalpha(str[i]) && \
					!ft_isdigit(str[i]) && str[i] != ' ')
			{
				if ((str[i] == ';' || str[++i] == '[') && ++i)
					i += skip_num(str + i);
				if (str[i] != ';')
					++i;
			}
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

void			sh_rewrite(const DSTRING *prompt, const DSTRING *buf,\
						const size_t index)
{
	struct winsize		term;
	int					len_p;

	ioctl(0, TIOCGWINSZ, &term);
	len_p = ft_color_strlen(prompt->txt, term);
	sh_clear_buf(term, len_p, index);
	ft_putstr_fd(prompt->txt, STDOUT_FILENO);
	sh_putstr_term(buf, term, len_p);
	if (buf->strlen != (ssize_t)index)
		sh_new_move_cors(buf, term, len_p, index);
	g_prebuf = buf->strlen;
	g_preind = index;
}
