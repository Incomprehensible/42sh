/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_cutters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/22 01:52:55 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

size_t	cut_quots(char *str)
{
	int		counter;
	size_t	len;

	counter = 0;
	len = 0;
	while (!(is_q(str[len])))
		len = (str[len] == '\\') ? len + 2 : ++len;
	while (is_q(str[len++]))
		counter++;
	while (counter && str[len] && !is_sep_no_space(str[len]))
	{
		if (is_q(str[len]))
			--counter;
		len = (str[len] == '\\') ? len + 2 : ++len;
	}
	return (len - 2);
}

size_t	cut_brackets(char *str)
{
	int		counter;
	size_t	len;
	size_t	br;

	counter = 0;
	len = 0;
	while (str[len] != '(')
		len = (str[len] == '\\') ? len + 2 : ++len;
	while (str[len++] == '(')
		counter++;
	br = 2;
	while (counter && str[len])
	{
		if (str[len] == '(')
			++counter;
		else if (str[len] == ')')
			--counter;
		len = (str[len] == '\\') ? len + 2 : ++len;
	}
	return (len - (br * 2));
}
