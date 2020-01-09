/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_portal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/10 22:36:32 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char			*assig_into_portal(char *str, t_dlist **tok, t_stx **tree)
{
	if (*str == '$')
	{
		if (!(str = parse_deref(str, tok, tree, 1)))
			return (NULL);
	}
	else if (*str == '"')
	{
		if (!(str = parse_dquotes(str, tok, tree, 0)))
			return (NULL);
	}
	else if (*str == '\'')
	{
		if (!(str = parse_apofs(str, tok, tree, 0)))
			return (NULL);
	}
	return (str);
}

static short	valid_assig(t_dlist *token_list)
{
	if (token_list && TOK_TYPE == TK_EXPR)
	{
		TOK_TYPE = TK_VAR;
		return (1);
	}
	return (0);
}

static short	chck_follow_pipe(t_dlist *token_list)
{
	if (token_list->prev)
		token_list = token_list->prev;
	else
		return (1);
	while (token_list && TOK_TYPE == TK_EMPTY)
		token_list = token_list->prev;
	if (!token_list || TOK_TYPE != TK_PIPE)
		return (1);
	return (0);
}

static char		*check_envar(char *str, t_dlist **tok, t_stx **tree)
{
	if (tok[1]->content && valid_assig(tok[1]))
	{
		if (!chck_follow_pipe(tok[1]))
			return (NULL);
		str = into_envar(str, tok, tree);
	}
	return (str);
}

char			*check_subbranch(char *str, t_dlist **tok, t_stx **tree)
{
	if (is_sep_no_space(*str))
		str = parse_sep(str, tok, 0);
	else if (ft_isspace(*str))
		str = parse_empty(str, 0x0, tok);
	else if (*str == '#')
		str = skip_comment(str);
	else if (*str == '=' || *str == '+')
		return (check_envar(str, tok, tree));
	else if (*str == '\'' && check_branch(str, tree[APOFS]))
		str = block_pass(APOFS, str, tok, tree);
	else if (*str == '"' && check_branch(str, tree[DQUOTS]))
		str = block_pass(DQUOTS, str, tok, tree);
	else if ((*str == '$' || *str == '~') && is_tok_deref(str))
		str = parse_deref(str, tok, tree, 1);
	else if (*str == '(' && check_branch(str, tree[MATHS]))
		str = block_pass(MATHS, str, tok, tree);
	else if (*str == '(' && check_branch(str, tree[SUBSHS]))
		str = block_pass(SUBSHS, str, tok, tree);
	else if (check_branch(str, tree[HERED]))
		str = block_pass(HERED, str, tok, tree);
	else if (is_token_here(str, "exec"))
		str = parse_exec(str, tok);
	else
		str = parse_redir(str, tok, tree, 0);
	return (str);
}
