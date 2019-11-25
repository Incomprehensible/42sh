/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_graph_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/25 18:39:34 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static short	pull_type(char *str)
{
	char	*buf[5];
	short	i;
	short	j;

	i = 0;
	j = 0;
	buf[0] = "if";
	buf[1] = "while";
	buf[2] = "until";
	buf[3] = "for";
	buf[4] = NULL;
	while (buf[i])
	{
		while (str[j] && (!is_separator(str[j])) && str[j] == buf[i][j])
			j++;
		if (!buf[i][j] && (!str[j] || is_separator(str[j])))
			return (i);
		i++;
		j = 0;
	}
	return (-1);
}

short	graph_type(char *str)
{
	static short	tmp;
	short			choice;

	choice = pull_type(str);
	if (!tmp)
		tmp = choice + 1;
	if (choice < 0)
		return (choice);
	return (choice);
}

static char		*new_graph(char *s, t_dlist **tok, t_stx **tr)
{
	short	i;

	if ((i = graph_type(s)) != -1)
		s = parse_scripts(s, tok, tr, i);
	return (s);
}

char			*into_the_portal(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
	if (!g || !(*s))
		return (s);
	if (g->type & TK_EXPR)
		s = new_graph(s, tok, tr);
	return (s);
}
