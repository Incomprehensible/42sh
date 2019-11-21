/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/22 00:55:02 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char	*parse_assig_block(char *str, t_dlist **tok, t_stx **tree)
{
	size_t	j;
	short	i;

	j = 0;
	i = 0;
	while (str[j] && !((!i && is_separator(str[j])) && str[j] != '"'))
	{
		if (str[j] == '\\' && ++j)
			i = 1;
		else if (!i && (str[j] == '$' || str[j] == '"'))
		{
			str += can_pull_tk(j, &str[j], tok, 0);
			j = 0;
			if (!(str = assig_into_portal(str, tok, tree)))
				return (NULL);
			if (sep_detected(tok[1]))
				return (str);
			continue ;
		}
		++j;
		i = 0;
	}
	return (str + can_pull_tk(j, &str[j], tok, 0));
}

char	*pull_expr1(char *patt, char *str, t_stx **tr, t_dlist **tok)
{
	char *new;

	if (check_branch(skip_spaces(str), tr[FLOWS]))
		return (str);
	if (layer_parse_two(patt, str))
	{
		if ((new = parse_comm(str, tok, tr, IN)) && new == str)
			return (NULL);
		if (!new)
			return (NULL);
		str = new;
	}
	return (str);
}

char	*pull_expr2(char *str, t_stx **tr, t_dlist **tok)
{
	char *new;

	if (check_branch(skip_spaces(str), tr[FLOWS]))
		return (str);
	if (check_branch(skip_spaces(str), tr[ENVAR]))
		str = parse_envar(str, tok, tr, 0);
	if (!str)
		return (NULL);
	if ((new = parse_comm(str, tok, tr, '\n')) && new == str)
		return (NULL);
	if (!new)
		return (NULL);
	str = new;
	if (!check_valid_sep(tok[1]))
		return (NULL);
	return (str);
}

short	got_in_seq(char sym, char *seq)
{
	while (*seq && *seq != '@')
	{
		if (*seq == sym)
			return (1);
		seq++;
	}
	return (0);
}

size_t	get_seq(char *str, char *meta)
{
	size_t i;

	i = 0;
	while (got_in_seq(*str, meta + 1))
	{
		str++;
		i++;
	}
	return (i);
}
