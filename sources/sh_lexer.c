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

char    *parse_empty(char *str, char *patt, t_dlist **tok)
{
    if (str && (*str == ' ' || *str == '\t'))
        make_token(tok, " ", TK_EMPTY);
    if (patt)
    {
        while ((*patt != '^' && *patt != '_') && *str == ' ')
            str++;
    }
    else
        while (*str && *str == ' ')
            str++;
    return (str);
}

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

static size_t get_seq(char *str)
{
    size_t i;

    i = 0;
    while (*str >= 48 && *str <= 57)
    {
        str++;
        i++;
    }
    return (i);
}

size_t    layer_parse_two(char *meta, char *str)
{
    size_t  i;
    char    *tmp;

    i = 0x0;
    tmp = str;
    while (*str && *meta && (*str == *meta || special_meta(*meta)))
    {
        if (*meta && special_meta(*meta))
        {
            str = process_reg(str, meta, "\0");
            if (str)
            {
                if (*meta == '@')
                    i = get_seq(str);
                while (tmp != str)
                {
                    tmp++;
                    if (*meta != '^' && *meta != '_' && *meta != '.' && *meta != ' ')
                        i++;
                }
            }
            if (!str)
                return (0);
            if (*meta == '@' || *meta == '!' || *meta == '.')
                meta = get_point(meta);
            meta++;
        }
        else
        {
            str++;
            meta++;
            i++;
        }
        if (!*meta || *meta == ' ')
            return (i);
    }
    return (*meta ? 0x0 : i);
}

char    *pull_expr1(char *patt, char *str, t_dlist **tok)
{
    char *start;
    char *new;
    size_t i;

    start = str;
    i = layer_parse_two(patt, str);
    if (i)
    {
        new = pull_token(start, --i);
        make_token(tok, new, TK_EXPR);
        str = str + i;
    }
    return (str);
}

char    *pull_expr2(char *str, t_stx **tr, t_dlist **tok)
{
    char *start;
    char *new;
    size_t i;

    i = 0x0;
    start = str;
    if (check_branch(str, tr[1]))
        return (str);
    while (*str && *str != ';')
    {
        str++;
        i++;
    }
    if (i)
    {
        new = pull_token(start, i);
        make_token(tok, new, TK_EXPR);
    }
    return (str);
}

short   graph_end(t_graph *g)
{
    if (!g->forward && !g->right && !g->left)
        return (1);
    return (0);
}

short   graph_forward_only(t_graph *g)
{
    if (g->forward && (!g->right && !g->left))
        return (1);
    return (0);
}

char   *check_subbranch(char *str,  t_dlist **tok, t_stx **tree, t_tk_type block)
{
    if (block == TK_FLOWS)
    {
        if (check_branch(str, tree[3]))
            str = block_pass(TK_MATHS, str, tok, tree);
        else if (*str && *str == '$' && !is_separator(*(str + 1)))
            str = get_deref(str, tree, tok);
        else if (check_branch(str, tree[7]))
            str = block_pass(TK_FUNCS, str, tok, tree);
        else if (*str == '(' && check_branch(str, tree[8]))
            str = block_pass(TK_SUBSHS, str, tok, tree);
    }
    else if (block == TK_EXPRS)
    {
        if ((*str == '\'' || *str == '"') && check_branch(str, tree[4]))
            str = block_pass(TK_QUOTS, str, tok, tree);
        else if (*str && *str == '$' && !is_separator(*(str + 1)))
            str = get_deref(str, tree, tok);
        else if (*str == '(' && check_branch(str, tree[8]))
            str = block_pass(TK_SUBSHS, str, tok, tree);
    }
    return (str);
}

char    *parse_sep(char *str, t_dlist **tok, t_stx **tr, short i)
{
    char *new;
    char *kek;

    if (!(*str))
        return (str);
    new = (char *)ft_memalloc(sizeof(char) * 3);
    while (i != 2 && str[i])
    {
        new[i] = str[i];
        i++;
    }
    i = -1;
    if (!ft_strcmp("&&", new))
        i = TK_AND;
    else if (!ft_strcmp("||", new))
        i = TK_OR;
    else if (*new == '|' && (new[1] = '\0'))
        i = TK_PIPE;
    else if (*new == '&' && (new[1] = '\0'))
        i = TK_BCKR_PS;
    else if (*str == ';' && (new[1] = '\0'))
        i = TK_SEP;
    else
        ft_strdel(&new);
    if (i != -1)
        make_token(tok, new, i);
    kek = *str ? ++str : 0x0;
    return (i > 0 ? kek : block_pass(find_token(tr, str), str, tok, tr));
}