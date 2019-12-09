/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_sep_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/09 13:35:34 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short		get_sep_type(char *new, t_dlist **tok, short i)
{
	i = -1;
	if (!ft_strcmp("&&", new))
		i = TK_AND;
	else if (!ft_strcmp("||", new))
		i = TK_OR;
	else if (*new == '|')
		i = TK_PIPE;
	else if (*new == ';' && !(new[1] = '\0'))
		i = TK_SEP;
	else if (*new == '\n' && !(new[1] = '\0'))
		i = TK_SEP;
	else if (*new == '&')
		i = make_offset(tok, TK_BCKR_PS);
	return (i);
}

char		*parse_sep(char *str, t_dlist **tok, short i)
{
	char	*new;
	short	j;

	if (!str || !(*str))
		return (str);
	new = (char *)ft_memalloc(sizeof(char) * 3);
	while (i != 2 && str[i])
	{
		new[i] = str[i];
		i++;
	}
	if ((j = get_sep_type(new, tok, 0)) < 0 || j != TK_SEP)
		ft_strdel(&new);
	if (j != -1)
	{
		if (j != TK_SEP)
			new = NULL;
		if (j != TK_BCKR_PS)
			make_token(tok, new, j);
		str = (j == TK_OR || j == TK_AND) ? str + 2 : ++str;
	}
	return (str);
}

char		*parse_empty(char *str, char *patt, t_dlist **tok)
{
	if (str && (*str == ' ' || *str == '\t' || *str == '\n'))
		make_token(tok, NULL, TK_EMPTY);
	if (patt)
	{
		while ((*patt != '^' && *patt != '_') &&
		((ft_isspace(*str) || *str == '\n')))
			str++;
	}
	else
		while (*str && (ft_isspace(*str) || *str == '\n'))
			str++;
	return (str);
}
