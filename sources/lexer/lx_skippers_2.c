/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_skippers_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/01 03:33:47 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*skip_spaces_nd_nl(char *str)
{
	while (*str && (ft_isspace(*str)))
		str++;
	return (str);
}

size_t	mirror_passes(char *str)
{
	size_t	i;

	i = 0;
	if (ft_isspace(str[i]))
		i++;
	i++;
	return (i);
}

char	*skip_spaces(char *str)
{
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	return (str);
}

char	*skip_brackets(char *str, char br)
{
	while (*str == br)
		str = (*str == '\\') ? str + 2 : ++str;
	return (str);
}
