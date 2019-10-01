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

//открывающая кавычка - пока идет кавычка и дальше не следует пробел и не str[i + 1] == число или буква или !
//выражение - пока идет разделитель идем вперед?  если бы раздедлитель читаем выражение до разделителя, потом идем вперед по разделителю до пробела ,
//в другом случае идем до пробела и по пробелу или до разделителя
//еслши все кавычки закрылись и идет пробел - матан завершился
//если во втором этапе кавычка выражения открылась и дальше следует пробел - синтакс еррор, нет выражения

short   back_ps_check(t_dlist *token_list)
{
    while (token_list)
    {
        while (token_list && TOK_TYPE != HOLE)
            token_list = token_list->next;
        if (token_list)
            token_list = token_list->next;
        while (token_list && TOK_TYPE == TK_EMPTY)
            token_list = token_list->next;
        if (!token_list)
            return (1);
        else if (TOK_TYPE != TK_SEP)
            return (0);
        token_list = token_list->next;
    }
    return (1);
}

short   is_sep_token(t_tk_type type)
{
    if (type == TK_SEP || type == TK_AND || type == TK_OR || type == TK_BCKR_PS || type == TK_PIPE)
        return (1);
    return (0);
}

short   seps_check(t_dlist *token_list)
{
    t_tk_type type;

    while (token_list)
    {
        if (is_sep_token(TOK_TYPE))
        {
            type = TOK_TYPE;
            if (token_list->next)
                token_list = token_list->next;
            while (token_list && TOK_TYPE == TK_EMPTY)
                token_list = token_list->next;
            if (!token_list || !token_list->next)
            {
                if (!token_list && type == TK_BCKR_PS)
                    return (0);
                if (token_list && !token_list->next && TOK_TYPE == TK_BCKR_PS)
                    return (0);
                if (token_list && TOK_TYPE == TK_SEP)
                    return (1);
                if (token_list && is_sep_token(TOK_TYPE) && is_sep_token((type)))
                    return (0);
                return (1);
            }
            if (is_sep_token(TOK_TYPE) && TOK_TYPE != TK_BCKR_PS)
                return (0);
        }
        token_list = token_list->next;
    }
    return (1);
}

short   math_seq_vals(char str)
{
    if (str != 37 && str != 42 && str != 43 && str != 45 && !(str >= 47 && str <= 57) && !(ft_isdigit(str)) && !(ft_isalpha(str)))
        return (0);
    return (1);
}

short   is_end(char str)
{
    if (str == '\'' || str == '"' || str == ')' || str == '(')
        return (1);
    return (0);
}

short   is_prefix(char str)
{
    if (str =='+' || str == '-')
        return (1);
    return (0);
}

short   is_operator(char str)
{
    if (str == 37 || str == 42 || str == 43 || str == 45 || str == 47)
        return (1);
    return (0);
}

short   is_expr(char c)
{
    if ((is_operator(c) || ft_isalpha(c) || ft_isdigit(c)))
        return (1);
    return (0);
}

char    *get_last_bracket(char *str)
{
    size_t i;

    i = 0;
    while (*str == '(')
    {
        str++;
        i++;
    }
    while (*str && !is_separator(*str) && i)
    {
        if (*str == '\'' || *str == '"')
            return (0);
        if (*str == ')')
            i--;
        str++;
    }
    return (i ? 0 : str);
}

char    *get_last_qu(char *str, char del)
{
    size_t i;

    i = 0;
    while (*str == del)
    {
        str++;
        i++;
    }
    while (*str && !is_separator(*str) && i)
    {
        if (*str != del && is_end(*str))
            return (0);
        if (*str == del)
            i--;
        str++;
    }
    return (i ? 0 : str);
}

char   *closed_br(char *str)
{
    size_t counter;
    short size;

    counter = 0;
    while (*str == '(')
    {
        str++;
        counter++;
    }
    if (counter < 2)
        return (0);
    while (*str && !is_sep_no_space(*str) && counter)
    {
        size = 0;
        if (*str == '(')
        {
            while (*str == '(')
            {
                str++;
                counter++;
                size++;
            }
            if (size < 2)
                return (0);
        }
        if (*str == ')')
        {
            while (*str == ')')
            {
                str++;
                counter--;
                size++;
            }
            if (size < 2)
                return (0);
        }
        else if (*str == '\'' || *str == '"')
            if (!(str = get_last_qu(str, *str)))
                return (0);
        str++;
    }
    if (is_separator(*str))
        str++;
    return (counter ? 0 : str);
}

char   *closed_q(char *str, char del)
{
    size_t counter;

    counter = 0;
    while (*str && *str == del)
        counter = (*(++str) == del) ? counter + 2 : ++counter;
    str = skip_spaces(str);
    while (*str && counter && !is_sep_no_space(*str))
    {
        if (*str && *str == del && is_expr(*(str + 1)))
            counter++;
        if (*str != del && *str != '(' && is_end(*str))
            return (0);
        if (*str == '(')
        {
            if (!(str = get_last_bracket(str)))
                return (0);
        }
        else if (*str == del)
            while (*str && *str == del && counter--)
                str++;
        else
            str++;
    }
    return (counter ? NULL : is_separator(*str));
}

char   *closed_dels(char *str)
{
    if (is_token_here(str, "let"))
    {
        str = ft_process_ignore(str, ".let%");
        str = skip_spaces(str);
        if (*str != '\'')
            return (str + ft_strlen(str));
        return (closed_q(str, '\''));
    }
    str = skip_spaces(str);
    if (*str != '(' && *(str + 1) != '(')
        return (0);
    return (closed_br(str));
}

//открывающая кавычка - пока идет кавычка и дальше не следует пробел и не str[i + 1] == число или буква или !
//выражение - пока идет разделитель идем вперед?  если бы раздедлитель читаем выражение до разделителя, потом идем вперед по разделителю до пробела ,
//в другом случае идем до пробела и по пробелу или до разделителя
//еслши все кавычки закрылись и идет пробел - матан завершился
//если во втором этапе кавычка выражения открылась и дальше следует пробел - синтакс еррор, нет выражения

size_t  valid_prefix(char *str)
{
    char op;
    size_t i;

    op = *str;
    i = 0;
    while (is_prefix(str[i]))
    {
        if (str[i] != op)
            return (0);
        i++;
    }
    if (!str[i] || (str[i] && str[i] == '\\'))
        return (0);
    return (i);
}

size_t brackets_operand(char *str)
{
    size_t i;
    size_t pass;

    i = 0;
    while (str[i] == '(' || str[i] == '\'')
        i++;
    pass = i;
    if (is_prefix(str[i]))
    {
        if (!valid_prefix(&str[i]))
            return (0);
        while (is_prefix(str[i]))
            i++;
    }
    if (str[i] && (ft_isdigit(str[i]) && ft_isalpha(str[i + 1])))
        return (0);
    while (str[i] != '(' && !is_end(str[i]))
    {
        if (is_operator(str[i]))
            return (0);
        if (str[i] && (!is_end(str[i]) && str[i] != ')' && !math_seq_vals(str[i])))
            return (0);
        if (str[i] == '\\')
            i++;
        i++;
    }
     while (pass-- && is_end(str[i]))
         i++;
     return (i);
}

size_t valid_operand(char *str, size_t i)
{
    size_t tmp;

    tmp = 0;
    str = skip_spaces(str);
    if (*str == '(' || *str == '\'' || *str == '"')
        return (brackets_operand(str));
    while (str[i] == '\\' || str[i] == ' ' || str[i] == '\t')
        i++;
    if (is_prefix(*str))
        if (!(tmp = valid_prefix(str)))
            return (0);
    i += tmp;
    if (str[i] && (ft_isdigit(str[i]) && ft_isalpha(str[i + 1])))
        return (0);
    if (str[i] && (ft_isspace(str[i]) || (str[i] != 33 && !math_seq_vals(str[i]))))
        return (0);
    while (!is_separator(str[i]) && !is_end(str[i]))
    {
        if (is_operator(str[i]))
            return (0);
        if (str[i] == '\\')
            i++;
        i++;
    }
    return (i);
}

size_t  valid_operator(char *str, size_t i)
{
    //str = skip_spaces(str);
    while (str[i] && (ft_isspace(str[i]) || is_end(str[i])))
        i++;
    if (!str[i] || str[i] == '\\' || !(is_operator(str[i])))
        return (0);
    i++;
    if (!str[i] || !ft_isspace(str[i]))
        return (0);
    return (i);
}

size_t  valid_math_seq(char *str)
{
    size_t  j;
    short   op;
    short   ch;

    ch = 0;
    j = 0;
    op = 0;
    str = skip_spaces(str);
    while (str[j] && !is_separator(str[j]))
    {
        if (is_operator(str[j]))
        {
            op = 1;
            if (str[j + 1] && (str[j + 1] == '*' || str[j + 1] == '%' || str[j + 1] == '/'))
                return (0);
        }
        else
        {
            op = 0;
            if (str[j] == '\\' && str[j] && (str[j + 1] == ' ' || str[j + 1] == '\t'))
            {
                j++;
                ch = 0;
            }
            else if (!op && str[j] != 33 && !(math_seq_vals(str[j])))
                return (0);
            else if (str[j] && ft_isdigit(str[j]) && ft_isalpha(str[j + 1]))
                return (0);
            else
                ch = 1;
        }
        j++;
    }
    return ((!op && ch) ? j : 0);
}

//size_t  valid_operator(char *str, size_t i)
//{
//    //str = skip_spaces(str);
//    if (str[i] == '\\')
//        i++;
//    if (!(is_operator(str[i])))
//        return (0);
//    while (str[i] && (is_operator(str[i]) || str[i] == '\\'))
//    {
//        if (str[i] == '\\' && (str[++i] == ' ' || str[i] == '\t'))
//            i++;
//        else
//            i++;
//        while (str[i] == ' ' || str[i] == '\t')
//            i++;
//    }
//    return (i);
//}

short valid_apof(char *str, size_t i)
{
    str = skip_spaces(str);
    if (str[i] == '\\')
        return (0);
    if (str[i] != '\'' && str[i] != '"')
        return (0);
    if (str[i] && str[i + 1] == '\\')
        return (0);
    return (1);
}

size_t valid_bracket(char *str, char *patt)
{
    size_t  i;
    char    br;

    i = 0;
    br = (*patt == '(') ? '(' : ')';
    str = skip_spaces(str);
    if (str[i] == '\\')
        return (0);
    if (str[i] != br)
        return (0);
    while (str[i] == br)
        i++;
    if (str[i] && str[i] == '\\')
        return (0);
    return (i);
}

short valid_sep(char *str)
{
    str = skip_spaces(str);
    if (!is_sep_no_space(*str) && *str != '"' && *str != '\'')
        return (0);
    return (1);
}

//size_t valid_operand(char *str, size_t i)
//{
//    short   mir;
//    short   ch;
//
//    i = 0;
//    mir = 0;
//    // str = skip_spaces(str);
//    ch = 1;
//    while (str[i] == '\\' || str[i] == ' ' || str[i] == '\t')
//        i++;
//    if (str[i] && (ft_isdigit(str[i]) && ft_isalpha(str[i + 1])))
//        return (0);
//    while (str[i] && !is_end(str[i]) && !is_operator(str[i]) && !is_separator(str[i]))
//    {
//        str = skip_spaces(str);
//        if (!mir && (str[i] == '(' || str[i] == ')'))
//            break;
//        else if (str[i] == '\\' && str[i++])
//        {
//            mir = 1;
//            i = ((str[i] == ' ' || str[i] == '\t') && !(ch = 0)) ? ++i : i;
//        }
//        else if (!(math_seq_vals(str[i])))
//            return (0);
//        else
//        {
//            mir = 0;
//            i++;
//            ch = 1;
//        }
//    }
//    return (ch ? i : 0);
//}