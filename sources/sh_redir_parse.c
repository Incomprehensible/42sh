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

char*   parse_hedoc(char *str, t_dlist **tok, t_stx **tree, short i)
{
    str = parse_empty(str, 0x0, tok);
    if (*str != '<')
    {
        while (*str && *str != '<')
        {
            str++;
            i++;
        }
        make_token(tok, pull_token(str - i, i - 1), TK_EXPR);
        i = 0;
        while (*str && *str == '<')
        {
            str++;
            i++;
        }
        make_token(tok, pull_token(str - i, i - 1), TK_HERED);
        i = 0;
        while (*str && !(is_separator(*str)))
        {
            str++;
            i++;
        }
        make_token(tok, pull_token(str - i, i - 1), TK_WORD);
    }
    return (parse_sep(str, tok, tree, 0));
}

static short is_sep_no_space(char str)
{
    if (str != '\n' && str != ';' && str != '&' && str != '|')
        return (0);
    return (1);
}

static char *fd_pull(t_graph *g, char *s, t_dlist **tok)
{
    short   i;
    char    *new;
    t_graph *start;

    start = g;
    s = parse_empty(s, 0x0, tok);
    while (g && !(i = layer_parse_two(g->patt, s)))
        g = g->next;
    if (i)
    {
        new = pull_token(s, i);
        make_token(tok, new, g->type);
        s = s + i;
    }
    else if (*s == '&' || (*s && *(s - 1) == ' ' && *(s - 2) == '&'))
    {
        make_token(tok, ft_strdup("-21"), start->type);
        s = s + 1;
    }
    return (s);
}

static void refactor_fd(char **new, char *meta, size_t i)
{
    short j;

    j = 0;
    while (new[i])
        new[i++] = &meta[j++];
}

static short    proceding_fd(char *str)
{
    while (*str && !(*str >=65 && *str <= 90) && !(*str >= 97 && *str <= 122) &&
    (!is_sep_no_space(*str)))
    {
        if (*str >= 48 && *str <= 57)
            return (1);
        str++;
    }
    return (0);
}

static char *redir_pull(t_graph *g, char *s, t_dlist **tok)
{
    short   i;
    char    *new;

    while (g && !(i = layer_parse_two(g->patt, s)))
        g = g->next;
    if (i)
    {
        new = pull_token(s, i);
        make_token(tok, new, g->type);
        s = s + i;
    }
    return (s);
}

//static char *redir_pull(t_graph *g, char *s, t_dlist **tok, short t)
//{
//    short   i;
//    short size;
//    char    *new;
//
//    while (g && !(i = layer_parse_two(g->patt, s)))
//        g = g->next;
//    size = t ? i + 2 : i;
//    if (i)
//    {
//        new = pull_token(s, size);
//        if (!t)
//        {
//            if (!proceding_fd(s))
//                refactor_fd(&new, "-21", i);
//            else
//                ft_bzero(&new[i], 2);
//        }
//        make_token(tok, new, g->type);
//        s = s + i;
//    }
//    return (s);
//}

//static char *second_redir_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
//{
//    short   i;
//    char    *new;
//
//    while (g && !(i = layer_parse_two(g->patt, s)))
//        g = g->next;
//    if (i)
//    {
//        new = pull_token(s, i + 2);
//        make_token(tok, new, g->type);
//        s = s + i;
//    }
//    return (s);
//}

static char *in_redir_blocks(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
    s = parse_empty(s, g->patt, tok);
    if (g->type == TK_FD)
        s = fd_pull(g, s, tok);
    else
    {
//        i = (g->type == TK_RD_W && (ft_strlen(g->patt) > 1)) ? 0 : 1;
        s = redir_pull(g, s, tok);
    }
    return (s);
}

static char *parse_expr(char *s, t_dlist **tok)
{
    size_t i;

    i = 0;
    while (*s && *s != '&' && *s != '<' && *s != '>' && *s != ';' && !(*s >= 48 && *s <= 57))
    {
        i++;
        s++;
    }
    if (i && (*(--s) == ' ' || *s == '\t'))
    {
        make_token(tok, pull_token(s - i + 1, i - 1), TK_EXPR);
        s = parse_empty(s, 0x0, tok);
    }
    return (s);
}

static char *close_fd(char *str, t_dlist **tok)
{
    make_token(tok, ft_strdup("close"), TK_FD);
    str += 1;
    return (str);
}

static char *redirect_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
    short i;
    char *new;

    s = parse_empty(s, g->patt, tok);
    if (!(*s))
        return (s);
    if (g->next)
        return (in_redir_blocks(g, s, tr, tok));
    else if (g->type == TK_EXPR)
        s = parse_expr(s, tok);
    else if (*s == '-')
        s = close_fd(s, tok);
    else
    {
        i = layer_parse_two(g->patt, s);
        if (i)
        {
            new = pull_token(s, i);
            make_token(tok, new, g->type);
            s = s + i;
        }
    }
    return (s);
}

char        *redir_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
    char *tmp;

//    if (!(*s) || is_sep_no_space(*s))
//        return (parse_sep(s, tok, tr, 0));
    if (!g || ((tmp = redirect_pull(g, s, tr, tok)) && tmp == s))
        return (tmp);
    s = tmp;
    if (!(*s) || is_sep_no_space(*s))
        return (parse_sep(s, tok, tr, 0));
    s = parse_empty(s, g->patt, tok);
    while (g->type == TK_EXPR && (tmp = redirect_pull(g, s, tr, tok)) && tmp != s)
        s = tmp;
    if (graph_forward_only(g))
        return (redir_traverse(g->forward, s, tok, tr));
    else
    {
        s = parse_empty(s, g->patt, tok);
        if ((tmp = redir_traverse(g->forward, s, tok, tr)) && tmp == s)
            if (((tmp = redir_traverse(g->left, s, tok, tr)) && tmp == s))
                return (redir_traverse(g->right, s, tok, tr));
    }
    return (tmp);
}

//проверять на экранирование
//в цикле expr_tk если есть экранирование то продолжаем
//pull exec here
char*   parse_redir(char *str, t_dlist **tok, t_stx **tree, short i)
{
    static t_graph *redir;
    char *tmp;

    str = parse_empty(str, 0x0, tok);
    tmp = str;
    if (!redir)
    {
        redir = redir_in();
        if ((*str == '<' || *str == '>') && *(str + 1) == '&')
            redir = redir->forward;
        else if (*str == '<' || *str == '>')
            redir = redir->left;
        else if ((*str >= 48 && *str <= 57) || *str == '&')
            redir = redir->forward->forward;
        //if we find > > <> redir = redir->forward;
        //else if we find &< etc redir = redir->left;
    }

    if ((str = redir_traverse(redir, str, tok, tree)) == tmp)
        return (block_pass(TK_EXPRS, str, tok, tree));
    return (str);
}
