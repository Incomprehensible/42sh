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
        make_token(tok, " ", empty_tk);
    while ((*patt != '^' && *patt != '_') && *str == ' ')
        str++;
    return (str);
}

char    *pull_token(char *str, size_t i)
{
    char *new;
    size_t tmp;

    tmp = i;
    new = (char *)ft_memalloc((i));
    while (i--)
        *new++ = *str++;
    return (new - tmp);
}

size_t    layer_parse_two(char *meta, char *str)
{
    size_t  i;
    char    *tmp;

    i = 0;
    tmp = str;
    //removed
//    while (*meta != '^' && *str == ' ')
//        str++;
    while (*str && *meta && (*str == *meta || special_meta(*meta)))
    {
        if (*meta && special_meta(*meta))
        {
            str = process_reg(str, meta, "\0");
            if (str && *str)
            {
                while (tmp != str)
                {
                    tmp++;
                    if (*meta != '^' && *meta != '_' && *meta != '.')
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
        if (!*meta)
            return (i);
    }
    return (*meta ? 0 : i);
}

char    *pull_expr1(char *patt, char *str, t_dlist **tok)
{
    char *start;
    char *new;
    size_t i;

    start = str;
    i = layer_parse_two(patt, str);
    //removed
//    while (*start == ' ')
//    {
//        start++;
//        str++;
//    }
    if (i)
    {
        new = pull_token(start, --i);
        make_token(tok, new, expr_tk);
        str = str + i;
    }
//    if (*str == ';')
//        str = parse_sep(str, tok, tr, 0);
    return (str);
}

char    *pull_expr2(char *str, t_stx **tr, t_dlist **tok)
{
    char *start;
    char *new;
    size_t i;

    i = 0;
    start = str;
    //removed
//    while (*str == ' ')
//    {
//        str++;
//        start++;
//    }
    if (check_branch(str, tr[1]))
        return (str);
        //return (block_pass(3, str, tok, tr));
    while (*str && *str != ';')
    {
        str++;
        i++;
    }
    if (i)
    {
        new = pull_token(start, i);
        make_token(tok, new, expr_tk);
    }
    //remove from here to lower
//    if (*str == ';')
//        str = parse_sep(str, tok, tr, 0);
    return (str);
}

//char    *scripts_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr, short p)
//{
//    static t_graph *start;
//    char *tmp;
//
//    if (!start)
//        start = g;
//    if (check_branch(s, tr[p]))
//        s = block_pass(p, s, tok, tr);
//    tmp = script_pull(g->patt, g->type, s, tr, tok);
//    //if ((tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp == s)
//        //return (scripts_traverse(start, s, tok, tr, p));
//    s = tmp;
//    while ((g->patt[0] != '^' || g->patt[0] != '_') && *s == ' ')
//        s++;
//    while (g->type == expr_tk && (tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp != s)
//        s = tmp;
//        //s = script_pull(start->patt, g->type, s);
//    if (g->forward)
//        scripts_traverse(g->forward, s, tok, tr, p);
//    if (g->right)
//        scripts_traverse(g->right, s, tok, tr, p);
//    if (g->left)
//        scripts_traverse(g->left, s, tok, tr, p);
//    else
//        return (s);
//}

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

//char    *scripts_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr, short p)
//{
//    static t_graph *start;
//    char *tmp;
//
//    if (!start)
//        start = g;
//    if (check_branch(s, tr[p]))
//        s = block_pass(p, s, tok, tr);
//    if (!g || ((tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp == s))
//        return (NULL);
//    s = tmp;
//    if (graph_end(g) && !(*s))
//        return (s);
////    while ((g->patt[0] != '^' || g->patt[0] != '_') && *s == ' ')
////        s++;
//    while (g && g->type == expr_tk && (tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp != s)
//        s = tmp;
//    if (graph_forward_only(g))
//        return (scripts_traverse(g->forward, s, tok, tr, p));
//    else
//    {
//        if (!(tmp = scripts_traverse(g->forward, s, tok, tr, p)))
//        {
//            if (!(tmp = scripts_traverse(g->right, s, tok, tr, p)) && !(tmp = scripts_traverse(g->left, s, tok, tr, p)))
//            {
//                if (!(tmp = scripts_traverse(start, s, tok, tr, p)))
//                    return (s);
//            }
//            //return (s);
//        }
//    }
//    return (tmp);
//}

char   *check_subbranch(char *str,  t_dlist **tok, t_stx **tree, short block)
{
    if (block == 1)
    {
        if (check_branch(str, tree[3]))
            str = block_pass(3, str, tok, tree);
        else if (*str && *str == '$' && !is_separator(*(str + 1)))
            str = get_deref(str, tree, tok);
//        else if (check_branch(str, tree[5]))
//            str = block_pass(5, str, tok, tree);
        else if (check_branch(str, tree[7]))
            str = block_pass(7, str, tok, tree);
        else if (*str == '(' && check_branch(str, tree[8]))
            str = block_pass(8, str, tok, tree);
    }
    //else if
    return (str);
}

//t_graph *broken_input(t_graph *g, short flag)
//{
//    static t_graph *last;
//
//    if (!flag)
//        return (last);
//    last = g;
//}

//30.08
//char    *scripts_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr, short p)
//{
//    static t_graph *start;
//    char *tmp;
//
//    if (!start || (tmp = script_checkout(s, tok, tr, p) && tmp != s && (s = tmp)))
//        start = g;
////    if (check_branch(s, tr[p]))
////        s = block_pass(p, s, tok, tr);
//    s = check_subbranch(s, tr, 1);
//    if (!g || ((tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp == s))
//        return (s);
//    s = tmp;
//    //shorten code here
//    if (graph_end(g) || !(*s))
//        return (s);
////    while ((g->patt[0] != '^' || g->patt[0] != '_') && *s == ' ')
////        s++;
//    while (g && g->type == expr_tk && (tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp != s)
//        s = tmp;
//    if (graph_forward_only(g))
//        return (scripts_traverse(g->forward, s, tok, tr, p));
//    else
//    {
//        if ((tmp = scripts_traverse(g->forward, s, tok, tr, p)) && tmp == s)
//        {
//            if (((tmp = scripts_traverse(g->right, s, tok, tr, p)) && tmp == s) && ((tmp = scripts_traverse(g->left, s, tok, tr, p)) && tmp == s))
//                return (scripts_traverse(start, s, tok, tr, p));
//        }
//    }
//    return (tmp);
//}

//static short    graph_ended(t_graph *g)
//{
//    if (g->type != fi_tk && g->type != done_tk)
//        return (0);
//    return (1);
//}

//char   *check_input(t_graph *g, char *s)
//{
//    if (!g || graph_end(g))
//        return (s);
//    if (!graph_ended(g) && !(*s))
//    {
//        broken_input(g, 1);
//        return (s);
//    }
//    return (NULL);
//}

// Working version 1.09
//char    *scripts_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
//{
//    static t_graph *start;
//    char *tmp;
//
//    while (*s == ' ' && g->patt[0] != '^')
//        s++;
//    if (!start || ((tmp = new_graph(g, s, tok, tr)) && tmp != s && (s = tmp)))
//        start = g;
//    s = check_subbranch(s, tok, tr, 1);
//    if (!g || ((tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp == s) || !(*tmp))
//        return (check_input(g, tmp));
//        //return (s);
//    s = tmp;
////    if (graph_end(g))
////        return (s);
//    while ((g->type == expr_tk || g->type == fi_tk || g->type == done_tk) && (tmp = script_pull(g->patt, g->type, s, tr, tok)) && tmp != s)
//        s = tmp;
//    if (graph_forward_only(g))
//        return (scripts_traverse(g->forward, s, tok, tr));
//    else
//    {
//        while (*s == ' ' && g->patt[0] != '^')
//        {
//            s++;
//            tmp++;
//        }
//        if ((tmp = scripts_traverse(g->forward, s, tok, tr)) && tmp == s)
//        {
//            if (((tmp = scripts_traverse(g->right, s, tok, tr)) && tmp == s) && ((tmp = scripts_traverse(g->left, s, tok, tr)) && tmp == s))
//                return (scripts_traverse(start, s, tok, tr));
//        }
//    }
//    return (tmp);
//}

//OLD VERSION
//char    *parse_sep(char *str, t_dlist **tok, t_stx **tr, short i)
//{
//    char *new;
//    char *kek;
//
//    if (!(*str))
//        return (str);
//    new = (char *)ft_memalloc(sizeof(char) * 3);
//    while (i != 2 && str[i])
//    {
//        new[i] = str[i];
//        i++;
//    }
//    i = 0;
//    if (!ft_strcmp("&&", new) || !ft_strcmp("||", new))
//    {
//        make_token(tok, new, sep_tk);
//        i = 1;
//    }
//    else if (*str == ';' || *str == '|' || *str == '&')
//    {
//        new[1] = '\0';
//        make_token(tok, new, sep_tk);
//        i = 1;
//    }
//    else
//        ft_strdel(&new);
//    kek = *str ? ++str : '\0';
//    return (i ? kek : block_pass(find_token(tr, str), str, tok, tr));
//}

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
        i = and_tk;
    else if (!ft_strcmp("||", new))
        i = or_tk;
    else if (*new == '|' && (new[1] = '\0'))
        i = pipe_tk;
    else if (*new == '&' && (new[1] = '\0'))
        i = bckr_ps_tk;
    else if (*str == ';' && (new[1] = '\0'))
        i = sep_tk;
    else
        ft_strdel(&new);
    if (i != -1)
        make_token(tok, new, i);
    kek = *str ? ++str : '\0';
    return (i > 0 ? kek : block_pass(find_token(tr, str), str, tok, tr));
}

//t_graph *switch_choice(t_graph *choice, short i)
//{
//    if (i == 1)
//    {
//        if (choice->forward)
//            return (choice->forward);
//    }
//    else if (i == 2)
//    {
//        if (choice->right)
//            return (choice->right);
//    }
//    else
//    {
//        if (choice->left)
//            return (choice->left);
//    }
//    return (NULL);
//}



//char*   parse_scripts(char *str, t_dlist **tok, t_stx **tree)
//{
//    static t_graph *script_if;
//    static t_graph *script_while;
//    static t_graph *script_for;
//    static t_graph *choice;
//
//    if (!script_if)
//        script_if = if_script_in();
//    if (!script_while)
//        script_while = while_script_in();
//    if (!script_for)
//        script_for = for_script_in();
//    choice = script_checkout(str, tok, tree, 7);
//    if ((str = scripts_traverse(script, str, tok, tree, 7)) == tmp)
//        str = block_pass(find_token(tree, str), str, tok, tree);
//    return (str);
//}
