/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_bash_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/12 22:29:46 by bomanyte         ###   ########.fr       */
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

static short    special_condition(char *patt)
{
    patt = skip_spaces(patt);
    if (*patt == '!')
        return (1);
    return (0);
}

static size_t validate_simple_struct(char *s, size_t i, size_t br)
{
    size_t tmp;

    while (s[br] == '(')
        br++;
    tmp = br;
    s += i;
    i = 0;
    while (ft_isspace(s[i]))
        i++;
    while (s[i] && s[i] != ')')
    {
        if (s[i] == ';')
            return (0);
        if (s[i] == '\\')
            i++;
        i++;
    }
    while (s[i] == ')')
    {
        i++;
        br--;
    }
    if (br)
        return (0);
    return (i - tmp);
}

static size_t validate_math_struct(char *s, size_t i, short pass)
{
    while (s[i] && s[i] != ')')
    {
        while (ft_isspace(s[i]))
            i++;
        while (s[i] && s[i] != ';' && s[i] != ')')
        {
            if (s[i] == '\\')
                i++;
            i++;
        }
        if (!s[i] || (s[i] != ';' && pass < 2) || (s[i] != ')' && pass == 2))
            return (0);
        pass = (s[i] == ';') ? ++pass : pass;
        i++;
    }
    if (s[i] == ')' && s[i + 1] == ')')
        return (0);
    return (pass == 2 ? i - 1 : 0);
}

short   validate_math_condition(char *tmp)
{
    return (1);
}

short   validate_math_stx(char *s)
{
    size_t j;
    size_t k;
    char    *tmp;

    while (*s != ')')
    {
        j = 0;
        s = skip_spaces(s);
        while (*s != ';' && *s != ')')
        {
            if (*s == '\\')
            {
                s++;
                j++;
            }
            s++;
            j++;
        }
        k = remove_spaces(s - 1, j + 1);
        tmp = (char *)ft_memalloc(j - k + 1);
        ft_strlcat(tmp, s - j, j - k + 1);
        if (!validate_math_condition(tmp))
        {
            free(tmp);
            return (0);
        }
        s++;
        free(tmp);
    }
    return (1);
}

char    *pull_legit_math(char *s, t_dlist **tok, size_t i, short tp)
{
    size_t k;

    if (tp == FORMATH)
    {
        if (!(i = validate_math_struct(s + i, 0, 0)))
            return (NULL);
    }
    else
        if (!(i = validate_simple_struct(s, i, 0)))
            return (NULL);
    while (*s == '(' || ft_isspace(*s))
        s = (*s == '\\') ? s + 2 : ++s;
    if (!validate_math_stx(s))
        return (NULL);
    k = remove_spaces(s + i - 1, i);
    make_token(tok, pull_token(s, i - k), TK_MATH);
    s += i;
    while (*s == ')' || (ft_isspace(*s) && *s != '\n'))
        s = (*s == '\\') ? s + 2 : ++s;
    if (*s != ';' && *s != '\n')
        return (NULL);
    return (parse_sep(s, tok, 0));
}

char    *pull_math(char *s, t_dlist **tok, short tp)
{
    size_t i;

    i = 0;
    if (!layer_parse_two("((x))", s))
        return (s);
    while (s[i] && s[i] == '(')
        i = (s[i] == '\\') ? i + 2 : ++i;
    if ((i != 2 && tp == FORMATH) || (i < 2 && tp == MATH_NT))
        return (0);
    while (ft_isspace(s[i]))
        i = (s[i] == '\\') ? i + 2 : ++i;
    s = pull_legit_math(s, tok, i, tp);
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
        // if (tp != TK_IN)
        //     make_token(tok, pull_token(s, i - space), tp);
        s = s + i;
    }
    return (s);
}

static char	*pull_break_cont(char *s, t_tk_type tp, t_stx **tr, t_dlist **tok)
{
	char *tmp;

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

char    *script_pull(char *patt, t_tk_type tp, char *s, t_stx **tr, t_dlist **tok)
{
    if (!(*s))
        return (s);
    if (tp == TK_SEP)
        return (parse_sep(s, tok, 0));
    if (tp == GLUE)
        return (make_it_glue(s, tr, tok));
    if (tp == TK_EXPR || tp == TK_VAR)
    {
        if (tp == TK_EXPR && is_token_here(skip_spaces(s), "break"))
            return (pull_break_cont(s, TK_BREAK, tr, tok));
        else if (tp == TK_EXPR && is_token_here(skip_spaces(s), "continue"))
            return (pull_break_cont(s, TK_CONTIN, tr, tok));
        else
            return (((special_condition(patt)) ? pull_expr1(patt, s, tr, tok) : pull_expr2(s, tr, tok)));
    }
    else if (tp == MATH_NT || tp == FORMATH)
        return (pull_math(s, tok, tp));
    else
        s = normal_token(patt, tp, s, tok);
	if (*s == ' ' || *s == '\t') 
		s = parse_empty(s, 0x0, tok);
    if (is_sep_no_space(*s) && (tp != TK_DONE && tp != TK_FI))
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

static char *into_the_portal(t_graph *g, char *s, t_dlist **tok, t_stx **tr, short i)
{
    if (!g || !(*s))
        return (s);
    if (g->type & TK_EXPR)
        s = new_graph(s, tok, tr);
    return (s);
}

char    *get_script(t_graph *g, char *s, t_dlist **tok, t_stx **tr, short i)
{
    char *tmp;

    if ((tmp = into_the_portal(g, s, tok, tr, i)) != s && tmp)
        return (tmp);
    return (script_pull(g->patt, g->type, s, tr, tok));
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
    if (!s || !g || ((tmp = get_script(g, s, tok, tr, sig)) && tmp == s) || !tmp)
        return ((sig && tmp) ? s : NULL);
    s = pull_empty(g, tmp, tok);
    sig = 1;
	while (*s && g->type == TK_EXPR && ((tmp = get_script(g, s, tok, tr, sig)) != s) && tmp)
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
	DBG_PRINT_TOKENS(tok[0]);
    return (str);
}