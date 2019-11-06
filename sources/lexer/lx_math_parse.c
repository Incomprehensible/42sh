/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_math_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/06 18:48:40 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

//открывающая кавычка - пока идет кавычка и дальше не следует пробел и не str[i + 1] == число или буква или !
//выражение - пока идет разделитель идем вперед?  если бы раздедлитель читаем выражение до разделителя, потом идем вперед по разделителю до пробела ,
//в другом случае идем до пробела и по пробелу или до разделителя
//еслши все кавычки закрылись и идет пробел - матан завершился
//если во втором этапе кавычка выражения открылась и дальше следует пробел - синтакс еррор, нет выражения

short   math_forward_only(t_graph *g)
{
    if (g->forward && !g->left)
        return (1);
    return (0);
}

size_t   math_pull(char *str, t_graph *math)
{
    if (math->type == MATH_SEQ)
        return (valid_math_seq(str));
    else if (math->type == OPERAND)
        return (valid_operand(str, 0));
    else if (math->type == OPERATOR)
        return (valid_operator(str, 0));
    else if (math->type == APOF || math->type == APOF_END)
        return (valid_apof(str, 0));
    else if (math->type == BRACKET || math->type == BRACKET_END)
        return (valid_bracket(str, math->patt));
    else if (math->type == TK_SEP)
        return (valid_sep(str));
    return ((*str == ' ' || *str == '\t') ? 1 : 0);
}

short   math_ended(t_tk_type type)
{
    if (type == TK_SEP || type == TK_EMPTY)
        return (1);
    return (0);
}

short   math_traverse(char *s, t_graph *math, size_t pass)
{
    size_t          tmp;

    s = (math->type == TK_EMPTY) ? s : skip_spaces(s);
    if (!math || !(tmp = math_pull(s + pass, math)))
        return (0);
    pass = tmp;
    if (math_ended(math->type))
        return (math->type == TK_EMPTY ? -1 : tmp);
    if (math_forward_only(math))
        return (math_traverse(s + pass, math->forward, 0));
    else if (!(tmp = math_traverse(s + pass, math->forward, 0)))
        if (!(tmp = math_traverse(s + pass, math->left, 0)))
            return (0);
    return (tmp);
}

short    is_q(char c)
{
    if (c != '\'' && c != '"')
        return (0);
    return (1);
}

short   validate_math(char *str, short i)
{
    static t_graph  *math[2];
    t_graph         *choice;

    if (!math[0])
        math[0] = let_math_in();
    if (!math[1])
        math[1] = brackets_math_in();
    choice = math[1];
    if (is_token_here(str, "let"))
    {
        choice = math[0];
        if ((str = ft_process_ignore(str, ".let%")) && !(is_q(*(str = skip_spaces(str)))))
            choice = choice->next;
    }
    if (!(i = math_traverse(str, choice, 0)))
        return (0);
    return (i);
}

size_t   cut_quots(char *str)
{
    int     counter;
    size_t  len;

    counter = 0;
    len = 0;
    while (!(is_q(str[len])))
        len = (str[len] == '\\') ? len + 2 : ++len;
    while (is_q(str[len++]))
        counter++;
    while (counter && str[len] && !is_sep_no_space(str[len]))
    {
        if (is_q(str[len]))
            --counter;
        len = (str[len] == '\\') ? len + 2 : ++len;
    }
    return (len - 2);
}

char    *skip_brackets(char *str, char br)
{
    while (*str == br)
        str = (*str == '\\') ? str + 2 : ++str;
    return (str);
}

size_t   cut_brackets(char *str)
{
    int     counter;
    size_t  len;
    size_t br;

    counter = 0;
    len = 0;
    while (str[len] != '(')
        len = (str[len] == '\\') ? len + 2 : ++len;
    while (str[len++] == '(')
        counter++;
	br = 2;
    //br = counter;
    while (counter && str[len] && str[len] != '\n' && str[len] != ';')
    {
        if (str[len] == '(')
            ++counter;
        else if (str[len] == ')')
            --counter;
        len = (str[len] == '\\') ? len + 2 : ++len;
    }
    return (len - (br * 2));
}

size_t  cut_math_seq(char *str)
{
    size_t i;

    i = 0;
    while (str[i] && (str[i] != ' ' && str[i] != '\t') && !(is_sep_no_space(str[i])))
    {
        if (str[i] == '\\' && (str[i + 1] == ' ' || str[i + 1] == '\t'))
            i += 2;
        i++;
    }
    return (i);
}

char    *pull_math_seq(char *str, t_dlist **tok)
{
    size_t len;

    len = cut_math_seq(str);
    make_token(tok, pull_token(str, len), TK_MATH);
    str += len;
    return (str);
}

char    *pull_in_brackets(char br, char *str, t_dlist **tok)
{
    size_t len;

    len = (br == '(') ? cut_brackets(str) : cut_quots(str);
	str += 2;
    //str = skip_brackets(str, br);
    make_token(tok, pull_token(str, len), TK_MATH);
    br = (br == '(') ? ')' : br;
    return (skip_brackets(str + len, br));
}

static short tmp_validate_math(char *str, short i)
{
    return (1);
}

char   *parse_math(char *str, t_dlist **tok, t_stx **tree, short ind)
{
    char    br;

    if (!(*str))
        return (str);
    str = parse_empty(str, 0x0, tok);
    if (*str == '\\')
        return (block_pass(EXPRS, str, tok, tree));
    // if (!(closed_dels(str)) || !(ind = tmp_validate_math(str, 0)))
    //     return (NULL);
    if (is_token_here(str, "let"))
        str = ft_process_ignore(str, ".let%");
    str = skip_spaces(str);
    br = *str;
    if (br == '(' || br == '\'' || br == '"')
        str = pull_in_brackets(br, str, tok);
    else
        str = pull_math_seq(str, tok);
    return (ind < 0 ? "\0" : str);
}