/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/08/19 00:53:23 by bomanyte         ###   ########.fr       */
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

//char    *pull_expr1(char *patt, t_tk_type t, char *str, t_stx **tr, t_dlist **tok)
//{
//    char *new;
//    size_t i;
//
//    if (check_branch(str, tr[FLOWS]))
//        return (str);
//    if ((new = check_subbranch(str, tok, tr, FLOWS)) && new != str)
//        return (new);
//    if (is_token_here(str, "exec") && *str && *(str + 1) != '\\')
//        str = parse_empty(parse_exec(str, tok), 0x0, tok);
//    i = layer_parse_two(patt, str);
//    if (i)
//    {
//        new = pull_token(str, --i);
//        make_token(tok, new, t);
//        str = str + i;
//    }
//    return (str);
//}

//here we invoke func that parses content between two quotes
//there we split function in two - one removes quotes and inserts only string in that func, another parses content
//char    *pull_expr1(char *patt, t_tk_type t, char *str, t_stx **tr, t_dlist **tok)
//{
//    char *new;
//    size_t i;
//
//    if (check_branch(str, tr[FLOWS]))
//        return (str);
//    if ((new = check_subbranch(str, tok, tr, FLOWS)) && new != str)
//        return (new);
//    i = layer_parse_two(patt, str);
//    if (i)
//    {
//        new = pull_token(str, --i);
//        make_token(tok, new, t);
//        str = str + i;
//    }
//    return (str);
//}

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
//    if (ft_isspace(*str))
//        str = parse_empty(str, 0x0, tok);
    if (!check_valid_sep(tok[1]))
        return (NULL);
//    if (*str != '\n' && *str != ';')
//        return (NULL);
    return (str);
}

//char    *pull_expr2(char *str, t_stx **tr, t_dlist **tok)
//{
//    char *new;
//    size_t i;
//    short mir;
//
//    i = 0;
//    mir = 0;
//    if (check_branch(skip_spaces(str), tr[FLOWS]))
//        return (str);
//    if ((new = check_subbranch(str, tok, tr, FLOWS)) && new != str)
//        return (new);
//    if (is_token_here(str, "exec") && *str && *(str + 1) != '\\')
//        str = parse_empty(parse_exec(str, tok), 0x0, tok);
//    if ((new = parse_comm(str, tok, tr, 0)) && new == str)
//        return (NULL);
//    if (!new)
//        return (NULL);
//    str = new;
//    if (ft_isspace(*str))
//        str = parse_empty(str, 0x0, tok);
//    if (*str != '\n' && *str != ';')
//        return (NULL);
//    return (parse_sep(str, tok, 0));
//    while (!mir && *str && !is_sep_no_space(*str))
//    {
//        if (*str++ == '\\')
//            mir = 1;
//        else
//            mir = 0;
//        i++;
//    }
//    if (i)
//        make_token(tok, pull_token(str - i, i), TK_EXPR);
//    if (!(*str))
//        return (NULL);
//    return (str);
//}

//static char *flows_jump_into_portal(char *str, t_dlist **tok, t_stx **tree)
//{
//    if (check_branch(str, tree[MATHS]))
//        str = block_pass(MATHS, str, tok, tree);
//    else if (*str && *str == '$' && !is_separator(*(str + 1)))
//        str = parse_deref(str, tok, tree, 0);
//    else if (check_branch(str, tree[FUNCS]))
//        str = block_pass(FUNCS, str, tok, tree);
//    else if (*str == '(' && check_branch(str, tree[SUBSHS]))
//        str = block_pass(SUBSHS, str, tok, tree);
//    return (str);
//}

//static char *quots_jump_into_portal(char *str, t_dlist **tok, t_stx **tree)
//{
//    if (check_branch(str, tree[MATHS]))
//        str = block_pass(MATHS, str, tok, tree);
//    else if (*str && *str == '$' && !is_separator(*(str + 1)))
//        str = parse_deref(str, tok, tree, 0);
//    else if (check_branch(str, tree[FUNCS]))
//        str = block_pass(FUNCS, str, tok, tree);
//    else if (*str == '(' && check_branch(str, tree[SUBSHS]))
//        str = block_pass(SUBSHS, str, tok, tree);
//    return (str);
//}

static char *expr_jump_into_portal(char *str, t_dlist **tok, t_stx **tree)
{
    if (ft_isspace(*str))
        str = parse_empty(str, 0x0, tok);
    else if (is_sep_no_space(*str))
        str = parse_sep(str, tok, 0);
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
    return (str);
}

//??? maybe we just remove it?
char   *check_subbranch(char *str, t_dlist **tok, t_stx **tree, t_tk_type block)
{
//    if (block == FLOWS)
//        str = flows_jump_into_portal(str, tok, tree);
    if (block == EXPRS)
        str = expr_jump_into_portal(str, tok, tree);
//    else if (block == DQUOTS)
//        str = quots_jump_into_portal(str, tok, tree);
    return (str);
}