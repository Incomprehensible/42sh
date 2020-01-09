/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_move_insertion_point.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 11:21:26 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/28 17:25:37 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"

static int	skip_space(const char *str, int ind)
{
	while (str[ind] && str[ind] == ' ')
		++ind;
	return (ind);
}

static int	skip_alpha(const char *str, int ind)
{
	while (str[ind] && str[ind] != ' ')
		++ind;
	return (ind);
}

static int	skip_r_space(const char *str, int ind)
{
	while (ind > -1 && ind && (str[ind] == ' ' || str[ind] == '\0'))
		--ind;
	return (ind);
}

static int	skip_r_alpha(const char *str, int ind)
{
	while (ind > -1 && ind && str[ind] != ' ')
		--ind;
	return (ind);
}

int			sh_move_insertion_point(const DSTRING *str, int ind, const char ch)
{
	int		fl;

	fl = 0;
	if (ch == 0x02)
	{
		if (str->txt[ind] == ' ' || str->txt[ind] == '\0')
			ind = skip_r_alpha(str->txt, skip_r_space(str->txt, ind));
		else
			ind = skip_r_alpha(str->txt, skip_r_space(str->txt, ind - 1));
		if (ind != 0)
			++ind;
		if (ind < 0)
			return (0);
	}
	else if (ch == 0x06)
	{
		if (str->txt[ind] != ' ' && str->txt[ind] != '\0')
			ind = skip_space(str->txt, skip_alpha(str->txt, ind));
		else
			ind = skip_space(str->txt, ind);
		if (ind > str->strlen)
			return (str->strlen);
	}
	return (ind);
}
