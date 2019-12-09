/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_comm_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/09 13:10:44 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char			*parse_exec(char *str, t_dlist **tok)
{
	size_t	i;
	char	*new;

	i = 0;
	make_token(tok, pull_token(str, 4), TK_EXEC);
	str += 4;
	if (ft_isspace(*str))
		str = parse_empty(str, 0x0, tok);
	while (*str && !is_sep_no_space((*str)))
	{
		if ((*str && *str == '\\') || ((*str == '<' || *str == '>')
		&& *(str + 1) == '&'))
		{
			str++;
			i++;
		}
		str++;
		i++;
	}
	new = pull_token(str - i, i);
	make_token(tok, markup_station(new, TK_EXPR), TK_EXPR);
	free(new);
	return (parse_sep(str, tok, 0));
}

static short	redir_detected(char *str, t_stx **tree, short stop)
{
	if (stop == '<')
		return (0);
	str = (*str == '\\') ? str + 2 : str;
	while (*str && *str != stop && !(is_sep_no_space(*str)))
	{
		if (check_branch(str, tree[REDIR]))
			return (1);
		str = (*str == '\\') ? str + 2 : ++str;
	}
	return (0);
}

short			time_for_portal(char *str, t_stx **tree, short stop, size_t j)
{
	if ((ft_isdigit(*str) || *str == '&') && redir_detected(str, tree, stop))
		return (1);
	else if (is_sep_no_space(*str) || ft_isspace(*str))
		return (1);
	else if (*str == '#' || *str == '(' || *str == '"' || *str == '\'')
		return (1);
	else if (is_redir(str) || valid_deref(str, j) || \
	is_token_here(str, "exec"))
		return (1);
	return (0);
}

char			*parse_expr(char *str, t_dlist **tok, t_stx **tr, short sp)
{
	size_t	j;
	short	i;

	j = 0;
	i = 0;
	while (!(special_case(sp, &str[j])) && str[j])
	{
		if (str[j] == '\\' && (++j))
			i = 1;
		if (!i && !j && str[j] == '\n')
			str = parse_empty(&str[j], 0x0, tok);
		else if (!i && str[j] && time_for_portal(&str[j], tr, sp, j))
		{
			str += can_pull_tk(j, &str[j], tok, sp);
			if (!(str = check_subbranch(str, tok, tr)))
				return (NULL);
			if (!(j = 0) && sep_detected(tok[1]))
				return (str);
		}
		else
			++j;
		i = 0;
	}
	str += can_pull_tk(j, &str[j], tok, sp);
	return (str);
}

char			*parse_comm(char *str, t_dlist **tok, t_stx **tree, short stop)
{
	if (is_token_here(str, "exec") && *str && *(str + 1) != '\\')
		return (parse_exec(str, tok));
	str = parse_expr(str, tok, tree, stop);
	merge_into_expr(tok[0], tok);
	return (parse_sep(str, tok, 0));
}
