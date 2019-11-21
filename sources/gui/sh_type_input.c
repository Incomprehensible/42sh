/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_type_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 13:46:15 by gdaemoni          #+#    #+#             */
/*   Updated: 2019/11/20 19:52:53 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "sh_readline.h"

int					skip_space(char *str, int i, int max)
{
	while (str[i] == ' ' && i < max)
		++i;
	return (i);
}

int					skip_alpfha(char *str, int i, int max)
{
	while (ft_isalpha(str[i] && i < max))
		++i;
	return (i);
}

void				ind_inp_correct(const DSTRING *buf, t_indch *indch)
{
	int				i;

	indch->ind_slash = 0;
	// if (indch->type_inp != 1)
	// 	return ;
	i = indch->ind_inp;
	while (buf->txt[i])
	{
		if (buf->txt[i] == '/')
			indch->ind_slash = i + 1;
		if (buf->txt[i++] == ' ')
			return ;
	}
}

void				sh_type_input(DSTRING *buf, t_indch *indch)
{
	int			i;

	indch->type_inp = 0;
	i = skip_space(buf->txt, 0, indch->ind);
	indch->ind_inp = i;
	while (i < indch->ind)
	{
		i = skip_alpfha(buf->txt, i, indch->ind);
		if (buf->txt[i] == '\\' && (i += 2))
			continue ;
		if (ft_memchr(";|&(", buf->txt[i], 4) && !(indch->type_inp = 0))
		{
			i = skip_space(buf->txt, i + 1, indch->ind);
			indch->ind_inp = i;
			continue ;
		}
		if (buf->txt[i] == ' ' && (indch->type_inp = 1))
			indch->ind_inp = i + 1;
		else if (buf->txt[i] == '$' && (indch->type_inp = 2))
			indch->ind_inp = i + 1;
		else if (ft_memchr("*?[", buf->txt[i], 3) && (indch->type_inp = 3))
			indch->reg = i;
		++i;
	}
	ind_inp_correct(buf, indch);
}
