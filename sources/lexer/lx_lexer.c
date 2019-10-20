/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/14 02:31:46 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

char    *pull_token(char *str, size_t i)
{
    char *new;
    size_t tmp;

    tmp = i;
    new = (char *)ft_memalloc((i + 1));
    while (*str && i--)
        *new++ = *str++;
    return (new - tmp);
}

static short    dont_count(char *meta)
{
    if (*meta != '@' && *meta != '^' && *meta != '_' && *meta != '.' && *meta != ' ')
        return (0);
    return (1);
}

static short   can_finish(char *meta)
{
    if ((!*meta || *meta == ' ' || *meta == '_'))
        return (1);
    return (0);
}

size_t    layer_parse_two(char *meta, char *str)
{
    size_t  i;
    char    *tmp;

    i = 0;
    tmp = str;
    while (*str && *meta && (*str == *meta || special_meta(*meta)))
    {
        if (*meta && special_meta(*meta))
        {
            str = process_reg(str, meta);
            if (str)
            {
                if (*meta == '@')
                {
                    i = get_seq(str);
                    str += i;
                }
                while (tmp != str)
                {
                    tmp++;
                    i = (dont_count(meta)) ? i : ++i;
                }
            }
            if (!str)
                return (0);
            meta = (*meta == '@' || *meta == '!' || *meta == '.') ? get_point(meta) + 1 : ++meta;
        }
        else
        {
            str++;
            meta++;
            i++;
        }
        if (can_finish(meta))
            return (i);
    }
    return (*meta ? 0 : i);
}

char    *pull_expr1(char *patt, char *str, t_stx **tr, t_dlist **tok)
{
    char *new;

    if (check_branch(str, tr[FLOWS]))
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

char    *pull_expr2(char *str, t_stx **tr, t_dlist **tok)
{
    char *new;

    if (check_branch(skip_spaces(str), tr[FLOWS]))
        return (str);
    if ((new = parse_comm(str, tok, tr, '\n')) && new == str)
        return (NULL);
    if (!new)
        return (NULL);
    str = new;
    if (!check_valid_sep(tok[1]))
        return (NULL);
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

char   *check_subbranch(char *str, t_dlist **tok, t_stx **tree)
{
	if (is_sep_no_space(*str))
        str = parse_sep(str, tok, 0);
    else if (ft_isspace(*str))
        str = parse_empty(str, 0x0, tok);
	else if (*str == '#')
		str = skip_comment(str);
	else if (*str == '=' || *str == '+')
	{
		if (tok[1]->content && valid_assig(tok[1]))
		{
			if (!chck_follow_pipe(tok[1]))
        		return (NULL);
			str = into_envar(str, tok, tree);
		}
	}
    else if (*str == '\'' && check_branch(str, tree[APOFS]))
        str = block_pass(APOFS, str, tok, tree);
    else if (*str == '"' && check_branch(str, tree[DQUOTS]))
        str = block_pass(DQUOTS, str, tok, tree);
    else if (*str && *str == '$' && !is_separator(*(str + 1)))
        str = parse_deref(str, tok, tree, 0);
    else if (*str == '(' && check_branch(str, tree[MATHS]))
        str = block_pass(MATHS, str, tok, tree);
    else if (*str == '(' && check_branch(str, tree[SUBSHS]))
        str = block_pass(SUBSHS, str, tok, tree);
    else if (is_token_here(str, "exec"))
        str = parse_exec(str, tok);
    else
        str = parse_redir(str, tok, tree, 0);
    return (str);
}