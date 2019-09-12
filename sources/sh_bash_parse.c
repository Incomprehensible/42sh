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

char    *script_pull(char *patt, t_tk_type type, char *str, t_stx **tr, t_dlist **tok)
{
    size_t i;
    char *start;
    char *new;

    if (!(*str))
        return (str);
    str = parse_empty(str, patt, tok);
    if (type == TK_EXPR)
        str = ((ft_strchr(patt, '[') || ft_strchr(patt, '!') || ft_strstr(patt, "((")) ? pull_expr1(patt, str, tok) : pull_expr2(str, tr, tok));
    else
    {
        start = str;
        i = layer_parse_two(patt, str);
        if (i)
        {
            while (*start == ' ')
            {
                str++;
                start++;
            }
            new = pull_token(start, i);
            make_token(tok, new, type);
            str = str + i;
        }
    }
    if (*str == ';')
        str = parse_sep(str, tok, 0);
    return (str);
}

char    *new_graph(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
    short i;

    s = parse_empty(s, g->patt, tok);
    if ((i = graph_type(g, s)) && i != -1)
        s = parse_scripts(s, tok, tr, i);
    return (s);
}

static char *into_the_portal(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
    if (!g || !(*s))
        return (s);
    s = parse_empty(s, g->patt, tok);
    s = new_graph(g, s, tok, tr);
    if (!is_token_here(s, "do") && !is_token_here(s, "else"))
        s = check_subbranch(s, tok, tr, TK_FLOWS);
    return (s);
}

//changed
char        *scripts_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
    static short sig;
    char *tmp;

    s = into_the_portal(g, s, tok, tr);
    if (!g || ((tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp == s))
        return (sig ? tmp : NULL);
    s = tmp;
    s = parse_empty(s, g->patt, tok);
    sig = 1;
    while ((g->type == TK_EXPR || g->type == TK_FI || g->type == TK_DONE) && (tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp != s)
        s = tmp;
    if (graph_end(g) || !(*s))
        return (s);
    if (graph_forward_only(g))
        return (scripts_traverse(g->forward, s, tok, tr));
    else
    {
        if ((tmp = scripts_traverse(g->forward, s, tok, tr)) && tmp == s)
            if (((tmp = scripts_traverse(g->right, s, tok, tr)) && tmp == s) && (sig = 0))
                return (scripts_traverse(g->left, s, tok, tr));
    }
    return (tmp);
}

short   pull_type(char *str)
{
    char *buf[5];
    short   i;
    short   j;

    i = 0;
    j = 0;
    buf[0] = "if";
    buf[1] = "while";
    buf[2] = "until";
    buf[3] = "for";
    buf[4] = NULL;
    while (buf[i])
    {
        while (str[j] && (str[j] != ' ' && str[j] != '\t' && str[j] != '\n' && str[j] != ';') && str[j] == buf[i][j])
            j++;
        if (!buf[i][j] && (!str[j] || str[j] == ' ' || str[j] == '\t' || str[j] == '\n' || str[j] == ';'))
            return (i);
        i++;
        j = 0;
    }
    return (-1);
}

short   graph_type(t_graph *g, char *str)
{
    static short tmp;
    short choice;

    choice = pull_type(str);
    if (!tmp)
        tmp = choice + 1;
    if (choice < 0)
        return (choice);
    if (g)
    {
        if (tmp - 1 == choice)
            return (-1);
        tmp = choice + 1;
    }
    return (choice);
}

char*   parse_scripts(char *str, t_dlist **tok, t_stx **tree, short ind)
{
    static t_graph  *script[4];
    char *tmp;

    tmp = str;
    if (!script[0])
        script[0] = if_script_in();
    if (!script[1])
        script[1] = while_script_in();
    if (!script[2])
        script[2] = until_script_in();
    if (!script[3])
        script[3] = for_script_in();
    if (!ind)
        ind = graph_type(NULL, str);
    if ((str = scripts_traverse(script[ind], str, tok, tree)) == tmp)
        return (block_pass(TK_EXPRS, str, tok, tree));
    else if (!str)
        exit(unexpected_token());
    return (str);
}