/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_redir_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/07 07:02:47 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short is_sep_no_space(char str)
{
    if (str != '\n' && str != ';' && str != '|' && str != '&' && str != '\0')
        return (0);
    return (1);
}

static size_t decimals_proceed(char *str, char *meta)
{
    size_t  i;

    i = 0;
    if (*meta == '@')
    {
        if ((str = ft_process_seq(str, meta)))
            i = get_seq(str);
        if (!str)
            return (0);
        str += i;
    }
    meta = (*meta == '@') ? get_point(meta) + 1 : ++meta;
    while (*meta && *str && *meta == *str)
    {
        str++;
        meta++;
        i++;
    }
    if (!(*meta) && *str != '-')
        return (i);
    return (0);
}

static size_t parse_fd(char *meta, char *str)
{
    size_t  i;

    i = 0;
    while (str[i] && *meta && (str[i] == *meta || special_meta(*meta)))
    {
        if (*meta && special_meta(*meta))
            return (decimals_proceed(str, meta));
        else
        {
            str++;
            meta++;
            i++;
        }
        if (!(*meta))
            return (i);
    }
    return (*meta ? 0 : i);
}

static char *fd_pull(t_graph *g, char *s, t_dlist **tok)
{
    short   i;
    char    *new;

    while (g && !(i = parse_fd(g->patt, s)))
        g = g->next;
    if (i)
    {
        if (g->type == CLOSE)
            new = ft_strdup("-42");
        else
            new = pull_token(s, i);
        make_token(tok, new, TK_FD);
        s = s + i;
    }
    return (s);
}

static char *redir_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
    short   i;

    if (layer_parse_one("<(z)", s) || layer_parse_one(">(z)", s))
        return (parse_proc(s, tok, tr, PROF));
    while (g && !(i = layer_parse_two(g->patt, s)))
        g = g->next;
    if (i && i < 3)
    {
        if (*s == '&')
            make_token(tok, ft_strdup("-21"), TK_FD);
        make_token(tok, NULL, g->type);
        s = s + i;
    }
    return (s);
}

static char *in_redir_blocks(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
    if (g->type == TK_FD)
        s = fd_pull(g, s, tok);
    else if (g->type == TK_PROF_OUT)
    {
        if (layer_parse_one("<(z)", s) || layer_parse_one(">(z)", s))
            s = parse_proc(s, tok, tr, PROF);
    }
    else
        s = redir_pull(g, s, tr, tok);
    return (s);
}

size_t remove_spaces(char *str, size_t len)
{
    size_t spaces;

    spaces = 0;
    while (*str && len && ft_isspace(*str))
    {
        str--;
        spaces++;
        if (*str == '\\')
        {
            spaces--;
            break;
        }
        len--;
    }
    return (spaces);
}

static char *parse_expr(char *s, t_dlist **tok)
{
    size_t i;
    size_t space;
    short flag;

    i = 0;
    flag = 0;
    space = 0;
    while (*s && *s != '&' && *s != '<' && *s != '>' && *s != ';' && !(*s >= 48 && *s <= 57))
    {
        flag = (ft_isspace(*s)) ? flag : 1;
        space = (ft_isspace(*s)) ? ++space : space;
        i++;
        s++;
    }
    if (flag && i)
    {
        space = remove_spaces(s - 1, i);
        make_token(tok, pull_token(s - i, i - space), TK_EXPR);
		if (space)
			make_token(tok, NULL, TK_EMPTY);
        s = parse_empty(s, 0x0, tok);
    }
    return (flag ? s : s - i);
}

static char *redirect_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
    short i;

    if (!(*s))
        return (s);
    if (g->next)
        return (in_redir_blocks(g, s, tr, tok));
    else if (g->type == TK_EXPR)
        s = parse_expr(s, tok);
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

short   is_tok_redir(t_tk_type type, short id)
{
    if (!id)
    {
        if (type == TK_PROF_OUT || type == TK_PROF_IN || type == TK_RD_A ||
            type == TK_RD_RW || type == TK_RD_W || type == TK_RD_R || type == TK_FD ||
            type == TK_FILENAME)
            return (1);
        return (0);
    }
    if (type == TK_RD_A || type == TK_RD_RW || type == TK_RD_W || type == TK_RD_R)
        return (1);
    return (0);
}

static short    stop_point(t_tk_type tk, t_dlist *token_list)
{
    if (token_list && (TOK_TYPE == TK_PROF_OUT || TOK_TYPE == TK_PROF_IN ||
    TOK_TYPE == TK_PROC_IN || TOK_TYPE == TK_PROC_OUT))
        return (1);
    else if (is_tok_redir(tk, 1))
        return (0);
    return (1);
}

static short    prof_found(t_tk_type type, t_dlist *token_list)
{
    if (TOK_TYPE == TK_PROF_IN || TOK_TYPE == TK_PROF_OUT || TOK_TYPE == TK_PROC_IN
    || TOK_TYPE == TK_PROC_OUT)
        if (is_tok_redir(type, 1))
            return (1);
    return (0);
}

char        *redir_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
    char *tmp;
    static short sig;

    s = g ? parse_empty(s, g->patt, tok) : s;
    if (!g || ((tmp = redirect_pull(g, s, tr, tok)) && tmp == s) || !tmp)
        return (sig ? tmp : NULL);
    s = parse_empty(tmp, g->patt, tok);
	if (prof_found(g->type, tok[1]))
        g = g->forward;
    sig = 1;
    if ((!(*s) || is_sep_no_space(*s)) && (stop_point(g->type, tok[1])))
        return (s);
    if (graph_forward_only(g) && !(sig = 0))
        return (redir_traverse(g->forward, s, tok, tr));
    else
    {
        if ((tmp = redir_traverse(g->forward, s, tok, tr)) && tmp == s)
            if (((tmp = redir_traverse(g->left, s, tok, tr)) && tmp == s) && !(sig = 0))
                return (redir_traverse(g->right, s, tok, tr));
    }
    return (tmp);
}
char*   parse_redir(char *str, t_dlist **tok, t_stx **tree, short i)
{
    t_graph *redir;
    char *tmp;

    str = parse_empty(str, 0x0, tok);
    tmp = str;
    redir = redir_in();
    if (*str == '&' || ((*str == '<' || *str == '>') && *(str + 1) == '&'))
        redir = redir->left;
    else if (*str == '<' || *str == '>')
        redir = redir->right;
    else if ((*str >= 48 && *str <= 57))
        redir = redir->forward;
    if ((str = redir_traverse(redir, str, tok, tree)) == tmp)
        return (block_pass(EXPRS, str, tok, tree));
    if (!str)
        return (NULL);
    return (str);
}
