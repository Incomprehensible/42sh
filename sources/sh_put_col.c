/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_put_col.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/17 18:33:07 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/08/17 18:35:17 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_readline.h"
#include "sh_termcomand.h"

// 1. запомнить позицию курсора

// 4. вернуть курсор на мето
// 5. при повторном нажатии дописать путь или комнду

void		ft_strset(char **str, char c, size_t n)
{
	int		i;

	i = 0;
	(*str) = (char*)malloc(sizeof(char) * n + 1);
	while (i < n)
		(*str)[i++] = c;
	(*str)[i] = '\0';
}

DSTRING		*sh_get_space(int n)
{
	char	*str;
	DSTRING	*rez;
	
	if (n > 0)
	{
		ft_strset(&str, ' ', n);
		rez = dstr_new(str);
		free(str);
		return (rez);
	}
	return (dstr_new(""));
}

void			put_col(t_darr darr)
{
	int		col;
	int		i;
	int		j;
	int		max_len;
	int		cnt;
	DSTRING	*space;
	DSTRING	*colums;

	col = 6; /* get_coll */
	max_len = 25; /* get_max_len */
	i = 0;
	j = 0;
	cnt = 0;
	colums = dstr_new("\n");
	ft_putstr(SAVECAR);
	while (1)
	{
		if (i < darr.count)
		{
			space = sh_get_space(max_len - darr.strings[i]->strlen);
			dstr_insert_dstr(colums, darr.strings[i], colums->strlen);
		}
		dstr_insert_dstr(colums, space, colums->strlen);
		i += darr.count / col + j;
		cnt++;
		if (cnt == col && (cnt = 0) == 0)
		{
			j++;
			i = 0;
			dstr_insert_str(colums, "\n", colums->strlen);
		}
		if (i == darr.count)
			break ;
	}
	ft_putstr(colums->txt);
	ft_putstr(LOADCAR);
}