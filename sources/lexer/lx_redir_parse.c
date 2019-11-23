/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_redir_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 00:16:29 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static char		*in_redir_blocks(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
	if (g->type == TK_FD)
		s = fd_pull(g, s, tok);
	else if (g->type == TK_PROF_OUT)
	{
		if (layer_parse_one("<(z)", s) || layer_parse_one(">(z)", s))
		{
			if (tok[1] && redir_proceeded(tok[1]))
				return (parse_proc(s, tok, tr, PROF));
			return (parse_proc(s, tok, tr, PROC));
		}
	}
	else
		s = redir_pull(g, s, tr, tok);
	return (s);
}

static char		*get_expr(char *s, size_t i, t_dlist **tok, t_stx **tr)
{
	size_t	spaces;
	char	*new;

	spaces = remove_spaces(s - 1, i);
	new = pull_token(s - i, i - spaces);
	if (!(parse_comm(new, tok, tr, 0)))
	{
		free(new);
		return (NULL);
	}
	if (spaces)
		make_token(tok, NULL, TK_EMPTY);
	free(new);
	return (parse_empty(s, 0x0, tok));
}

static short	fd_proceed(size_t spaces, char *s)
{
	if (!spaces)
		return (0);
	if (*s >= 48 && *s <= 57)
	{
		while (*s && (*s >= 48 && *s <= 57))
			s++;
		if (*s == '&' || *s == '<' || *s == '>')
			return (1);
	}
	return (0);
}

static char		*parse_expr(char *s, t_dlist **tok, t_stx **tr)
{
	size_t	i;
	size_t	spaces;
	short	flag;

	i = 0;
	flag = 0;
	spaces = 0;
	while (*s && *s != '&' && *s != '<' && *s != '>' && *s != ';'
	&& !(fd_proceed(spaces, s)))
	{
		flag = (ft_isspace(*s)) ? flag : 1;
		spaces = (ft_isspace(*s)) ? 1 : 0;
		i++;
		s++;
	}
	if (flag && i)
		if (!(s = get_expr(s, i, tok, tr)))
			return (NULL);
	return (flag ? s : s - i);
}

char			*redirect_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
	short	i;

	if (!(*s))
		return (s);
	if (g->next)
		return (in_redir_blocks(g, s, tr, tok));
	else if (g->type == TK_EXPR)
		s = parse_expr(s, tok, tr);
	else
	{
		if (is_redir(s))
			return (s);
		if ((i = layer_parse_two(g->patt, s)))
		{
			make_token(tok, pull_token(s, i), g->type);
			s = s + i;
		}
	}
	return (s);
}
