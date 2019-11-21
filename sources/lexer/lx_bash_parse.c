/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_bash_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/22 02:09:33 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static short   pull_type(char *str)
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
        while (str[j] && (!is_separator(str[j])) && str[j] == buf[i][j])
            j++;
        if (!buf[i][j] && (!str[j] || is_separator(str[j])))
            return (i);
        i++;
        j = 0;
    }
    return (-1);
}

static short   graph_type(char *str)
{
    static short tmp;
    short choice;

    choice = pull_type(str);
    if (!tmp)
        tmp = choice + 1;
    if (choice < 0)
        return (choice);
    return (choice);
}

char    *pull_legit_math(char *s, t_dlist **tok, t_stx **tr)
{
    s = parse_math(s, tok, tr, 0);
	if (!s)
		return (NULL);
    while (*s == ')' || !is_sep_no_space(*s))
        s = (*s == '\\') ? s + 2 : ++s;
    if (*s != ';' && *s != '\n')
        return (NULL);
    return (parse_sep(s, tok, 0));
}

char    *pull_math(char *s, t_dlist **tok, t_stx **tr, short tp)
{
    if (!layer_parse_two("((w))", s))
        return (s);
    if (tp == FORMATH && !validate_triple_struct(s, 0))
        return (0);
	else if (tp == MATH_NT && !validate_simple_struct(s, 0))
		return (0);
    while (ft_isspace(*s))
        s = (*s == '\\') ? s + 2 : ++s;
    s = pull_legit_math(s, tok, tr);
    return (s);
}

static char *make_it_glue(char *s, t_stx **tr, t_dlist **tok)
{
    char *tmp;

    tmp = parse_str_block(s, tok, tr, '\n');
    if (!tmp || tmp == s)
        return (NULL);
    if (!sep_detected(tok[1]) || !check_valid_sep(tok[1]))
        return (NULL);
    return (tmp);
}

static char *normal_token(char *patt, t_tk_type tp, char *s, t_dlist **tok)
{
    size_t i;
    size_t  space;

    space = 0;
    if ((i = layer_parse_two(patt, s)))
    {
        if (ft_isspace(*(s + i - 1)))
            space = remove_spaces(s + i - 1, i);
		make_token(tok, NULL, tp);
        s = s + i;
    }
    return (s);
}

static char	*pull_break_cont(char *s, t_tk_type tp, t_dlist **tok)
{
	char	*tmp;

	tmp = NULL;
	if (tp == TK_BREAK)
		tmp = normal_token("break_", TK_BREAK, s, tok);
	else if (tp == TK_CONTIN)
		tmp = normal_token("continue_", TK_CONTIN, s, tok);
	if (tmp == s)
		return (s);
	s = tmp;
	if (*s == ' ' || *s == '\t')
		s = parse_empty(s, 0x0, tok);
	s = parse_sep(s, tok, 0);
	if (!sep_detected(tok[1]) || !check_valid_sep(tok[1]))
        return (NULL);
	return (s);
}

char    *expr_in(char *s, t_graph *g, t_stx **tr, t_dlist **tk)
{
	if (g->type != TK_VAR && is_token_here(skip_spaces(s), "break"))
        return (pull_break_cont(s, TK_BREAK, tk));
    else if (g->type != TK_VAR && is_token_here(skip_spaces(s), "continue"))
        return (pull_break_cont(s, TK_CONTIN, tk));
    else if (special_condition(g->patt))
        return (pull_expr1(g->patt, s, tr, tk));
    return (pull_expr2(s, tr, tk));
}

char    *script_pull(t_graph *g, char *s, t_stx **tr, t_dlist **tok)
{
    if (!(*s))
        return (s);
    if (g->type == TK_SEP)
        return (parse_sep(s, tok, 0));
    if (g->type == GLUE)
        return (make_it_glue(s, tr, tok));
    if (g->type == TK_EXPR || g->type == TK_VAR || g->type == EX)
        return (expr_in(s, g, tr, tok));
    else if (g->type == MATH_NT || g->type == FORMATH)
        return (pull_math(s, tok, tr, g->type));
    else
        s = normal_token(g->patt, g->type, s, tok);
	if (*s == ' ' || *s == '\t')
		s = parse_empty(s, 0x0, tok);
    if (is_sep_no_space(*s) && (g->type != TK_DONE && g->type != TK_FI))
        s = parse_sep(s, tok, 0);
    return (s);
}

static char    *new_graph(char *s, t_dlist **tok, t_stx **tr)
{
    short i;

    if ((i = graph_type(s)) != -1)
        s = parse_scripts(s, tok, tr, i);
    return (s);
}

static char *into_the_portal(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
    if (!g || !(*s))
        return (s);
    if (g->type & TK_EXPR)
        s = new_graph(s, tok, tr);
    return (s);
}

char    *get_script(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
    char *tmp;

    if ((tmp = into_the_portal(g, s, tok, tr)) != s && tmp)
        return (tmp);
    return (script_pull(g, s, tr, tok));
}

static char *pull_empty(t_graph *g, char *s, t_dlist **tok)
{
    if (g && g->type != TK_FI && g->type != TK_DONE && g->type != TK_SEP)
        s = parse_empty(s, g->patt, tok);
    else if (g && (*s == ' ' || *s == '\t'))
        s = parse_empty(s, g->patt, tok);
    return (s);
}

char        *scripts_traverse(t_graph *g, char *s, t_dlist **tok, t_stx **tr)
{
    static short sig;
    char *tmp;

    s = g ? pull_empty(g, s, tok) : s;
    if (!s || !g || ((tmp = get_script(g, s, tok, tr)) && tmp == s) || !tmp)
        return ((sig && tmp) ? s : NULL);
    s = pull_empty(g, tmp, tok);
    sig = 1;
	while (*s && (g->type == TK_EXPR || g->type == EX) && ((tmp = get_script(g, s, tok, tr)) != s) && tmp)
        s = parse_empty(tmp, g->patt, tok);
    if (!tmp)
        return (NULL);
    if (graph_end(g, s) && (sig = 1))
        return (s);
    if (graph_forward_only(g) && !(sig = 0))
        return (scripts_traverse(g->forward, s, tok, tr));
    else
    {
        if ((tmp = scripts_traverse(g->forward, s, tok, tr)) && tmp == s)
            if (((tmp = scripts_traverse(g->left, s, tok, tr)) && tmp == s))
                if (((tmp = scripts_traverse(g->right, s, tok, tr)) && tmp == s) && !(sig = 0))
                        return (NULL);
    }
    return (tmp);
}

char*   parse_scripts(char *str, t_dlist **tok, t_stx **tree, short ind)
{
    static t_graph  *script[4];
    char *tmp;

    if (!ind)
        str = parse_empty(str, 0x0, tok);
    tmp = str;
    if (!script[0])
    {
        script[0] = if_script_in();
        script[1] = while_script_in();
        script[2] = until_script_in();
        script[3] = for_script_in();
    }
    if (!ind)
        ind = graph_type(str);
    if ((str = scripts_traverse(script[ind], str, tok, tree)) == tmp)
        return (NULL);
    return (str);
}