/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_new_input_finished.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/25 16:39:14 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short   is_it_q(char c)
{
    if (c == '\'' || c == '"' || c == '`')
        return (1);
    return (0);
}

size_t   mirror_passes(char *str)
{
    size_t i;

    i = 0;
    if (ft_isspace(str[i]))
        i++;
    i++;
    return (i);
}

size_t  skip_field(char *str, char skip)
{
    size_t jump;

    jump = 1;
    while (str[jump] && str[jump] != skip)
    {
        if (str[jump] == '\\' && str[++jump])
            jump++;
        else
            jump++;
    }
    if (!str[jump])
        return (0);
    return (jump);
}

short   q_closed(char *str, char q, char q1, char q2)
{
    size_t jump;
    short times;

    times = 0;
    while (*str)
    {
        if (*str == '\\')
            str += mirror_passes(str);
        else
        {
            if (*str == q)
                times = (times) ? --times : ++times;
            if (!times && (*str == q1 || *str == q2))
            {
                if (!(jump = skip_field(str, *str)))
                    return (0);
                str += jump;
            }
            else if (*str == '(')
                str += skip_field(str, *str);
            str++;
        }
    }
    return (times ? 0 : 1);
}

short   is_it_br(char c)
{
    if (c == '(')
        return (1);
    return (0);
}

char	*br_ap_cls_n(char *str)
{
	while (*str)
	{
		if (*str == '\'')
			return (++str);
		if (*str++ == '\\' && *str == '\'')
			++str;
	}
	return (str);
}
char	*br_qt_cls_n(char *str, short *i);

char	*br_cls_n(char *str, short *i)
{
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\"')
			str = br_qt_cls_n(++str, i);
		else if (*str == '(' && (++(*i) || 1)) // i plus
			str = br_cls_n(++str, i);
		else if (*str == '\'')
			str = br_ap_cls_n(++str);
		else if (*str == ')' && (--(*i) || 1)) // i minus
			return (++str);
		else
			++str;
	}
	return (str);
}

char	*br_qt_cls_n(char *str, short *i)
{
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\"')
			return (++str);
		else if (*str == '$' && *(++str) == '(' && (++(*i) || 1)) // i plus
			str = br_cls_n(++str, i);
		else
			++str;
	}
	return (str);
}

short	br_closed_n(char *str, short i)
{
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\'' || *str == '\"' || *str == '(' || *str == ')')
			str = br_cls_n(str, &i);
		else
			++str;
	}
	return (i);
}

short   brackets_closed(char *str)
{
    short times;

    times = br_closed_n(str, 0);
    if (times)
    {
        PARSE_ERR = times < 0 ? PRO_SUBSH : PARSE_ERR;
        INPUT_NOT_OVER = times > 0 ? PRO_SUBSH : INPUT_NOT_OVER;
        return (times > 0 ? 0 : -1);
    }
    return (1);
}

static short    is_and_closed(char *str, size_t size)
{
    if (*(str + size - 1) == '&' && (size - 2) && *(str + size - 2) == '&')
    {
        if ((size - 3) && *(str + size - 2) == '\\')
            return (1);
        return (0);
    }
    return (1);
}

short   input_closed(char *str)
{
    int size;

    size = ft_strlen(str);
    if (*(str + size - 1) == '|' && (size - 2 >= 0) && *(str + size - 2) != '\\')
    {
        INPUT_NOT_OVER = *(str + size - 2) == '|' ? PRO_OR : PRO_PIPE;
        return (0);
    }
    if (*(str + size - 1) == '\\' && (size - 2 >= 0) && *(str + size - 2) != '\\')
    {
        INPUT_NOT_OVER = PRO_NONE;
        return (0);
    }
    if (!is_and_closed(str, size))
    {
        INPUT_NOT_OVER = PRO_AND;
        return (0);
    }
    return (1);
}

short   are_tokens_here(char *str)
{
    if (is_token_here(str, "while"))
        return (1);
    if (is_token_here(str, "for"))
        return (1);
    if (is_token_here(str, "until"))
        return (1);
    return (0);
}

static char *if_to_the_start(char *str)
{
	str = skip_spaces(str);
    while (*str && !(is_token_here(str, "if")) && !(is_token_here(str, "fi")))
    {
        if (*str == '\\')
            str += mirror_passes(str);
        else if (is_it_q(*str) || is_it_br(*str))
        {
            str += skip_field(str, *str);
            ++str;
        }
        else
        {
            while (*str && *str != '\n' && *str != ';')
                str++;
            if (*str)
                str++;
            str = skip_spaces(str);
        }
    }
    return (skip_spaces(str));
}

static char *cycle_to_the_start(char *str)
{
    while (*str && !(are_tokens_here(str)) && !(is_token_here(str, "done")))
    {
        if (*str == '\\')
            str += mirror_passes(str);
        else if (is_it_q(*str) || is_it_br(*str))
        {
            str += skip_field(str, *str);
            ++str;
        }
        else
        {
            while (*str && *str != '\n' && *str != ';')
                str++;
            if (*str)
                str++;
            str = skip_spaces(str);
        }
    }
    return (skip_spaces(str));
}

int  validate_ifs(char *str)
{
    int num;

    num = 0;
    while (*str)
    {
		if ((*str == '\n' || *str == ';') && ++str)
			str = if_to_the_start(str);
        if (*str == '\\')
            str += mirror_passes(str);
        else if (is_it_q(*str) || is_it_br(*str))
        {
			if (is_it_br(*str))
            	str += skip_field(str, ')');
			else
				str += skip_field(str, *str);
            ++str;
        }
        else
        {
            if (is_token_here(str, "if"))
                num++;
            else if (is_token_here(str, "fi"))
                num--;
            str++;
        }
    }
    return (num);
}

short   br_closed(char *str, char strt, char fin)
{
    short times;

    times = 0;
    while (*str)
    {
        if (*str == '\\')
            str += mirror_passes(str);
        else
        {
            if (*str == strt)
                times++;
            if (*str == fin)
                times--;
            if (*str != strt && (is_it_q(*str) || (strt != '(' && *str == '(')))
                str += (*str == '(') ? skip_field(str, ')') : skip_field(str, *str);
            str++;
        }
    }
    return (times);
}

char	*qa_qt_cls_n(char *str, int *q)
{
	short	dump;

	dump = 1;
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\"' && (--(*q) || 1))
			return (++str);
		else if (*str == '$' && *(++str) == '(') // i plus
			str = br_cls_n(++str, &dump);
		else
			++str;
	}
	return (str);
}

char	*qa_ap_cls_n(char *str, int *a)
{
	while (*str)
	{
		if (*str == '\'' && (--(*a) || 1))
			return (++str);
		if (*str++ == '\\' && *str == '\'')
			++str;
	}
	return (str);
}

char	*qa_cls_n(char *str, int *a, int *q)
{
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\'' && (++(*a) || 1))
			str = qa_ap_cls_n(++str, a);
		else if (*str == '\"' && (++(*q) || 1))
			str = qa_qt_cls_n(++str, q);
		else
			++str;
	}
	return (str);
}

short   quotes_closed(char *str)
{
    int		aps;
	int		qts;

	aps = 0;
	qts = 0;
	qa_cls_n(str, &aps, &qts);
	if (aps)
	{
		INPUT_NOT_OVER = PRO_APOF;
		return (0);
	}
	if (qts)
	{
		INPUT_NOT_OVER = PRO_DQUOT;
		return (0);
	}
	return (1);
}

int  validate_cycles(char *str, char *meta)
{
    int num;

    num = 0;
    while (*str)
    {
		if ((*str == '\n' || *str == ';') && ++str)
			str = cycle_to_the_start(str);
        if (*str == '\\')
            str += mirror_passes(str);
        else if (is_it_q(*str) || is_it_br(*str))
        {
            str += skip_field(str, *str);
            ++str;
        }
        else
        {
            if (are_tokens_here(str))
                num++;
            else if (is_token_here(str, meta))
                num--;
            str++;
        }
    }
    return (num);
}

short   scripts_closed(char *str)
{
	char		*tmp;

	str = skip_spaces(str);
	tmp = str;
	tmp = if_to_the_start(str);
    if (*tmp && validate_ifs(tmp) > 0)
        return (0);
    str = cycle_to_the_start(str);
    if (*str && validate_cycles(str, "done") > 0)
        return (0);
    return (1);
}

char    *skip_spaces_newline(char *str)
{
    while (*str && (*str == ' ' || *str == '\t' || *str == '\n'))
        str++;
    return (str);
}

short func_really_closed(char *str)
{
    short times;

    str = skip_spaces_newline(str);
    if (!(*str))
        return (0);
    if (*str == '{')
    {
        times = br_closed(str, '{', '}');
        if (times)
        {
            PARSE_ERR = times < 0 ? PRO_NONE : PARSE_ERR;
            return (times > 0 ? 0 : -1);
        }
        return (1);
    }
	if (*str)
		return (1);
    return (0);
}

short func_is_closed(char *str)
{
    size_t i;

    str = skip_spaces(str);
    if (!*str)
        return (1);
    if ((i = layer_parse_two("? ", str)))
        return (func_really_closed(str + i));
    return (1);
}

short   funcs_closed(char *str)
{
    ssize_t i;

    str = skip_spaces(str);
    while (*str)
    {
        if (is_token_here(str, "function"))
            return (func_is_closed(str + 8));
        else if ((i = layer_parse_two("?()_", str)))
            return (func_really_closed(str + i));
        else if (*str == '{')
        {
            if (!(i = func_really_closed(str)))
                return (2);
            PARSE_ERR = i < 0 ? PRO_NONE : PARSE_ERR;
            return (i);

        }
        str += (*str == '\\') ? mirror_passes(str) : 1;
    }
    return (1);
}

short   parse_error(void)
{
    if (PARSE_ERR == PRO_SUBSH)
        ft_putstr("42sh: parse error in subshell: unexpected ')'");
    else if (PARSE_ERR == PRO_SQU)
        ft_putstr("42sh: parse error: '[' didn't close");
    else if (PARSE_ERR == PRO_NONE)
        ft_putstr("42sh: parse error in function: unexpected '}'");
    else
        ft_putstr("42sh: parse error occured");
    PARSE_ERR = 1;
    return (-1);
}

short   input_finished(char *str)
{
    short id;

    if (!input_closed(str))
        return (0);
    id = brackets_closed(str);
    if (id != 1)
        return (id ? parse_error() : 0);
    id = quotes_closed(str);
    if (id != 1)
        return (id ? parse_error() : 0);
    if (!scripts_closed(str))
    {
        INPUT_NOT_OVER = PRO_NONE;
        return (0);
    }
    id = funcs_closed(str);
    if (id != 1)
    {
        INPUT_NOT_OVER = id ? INPUT_NOT_OVER : PRO_NONE;
        INPUT_NOT_OVER = id == 2 ? PRO_LAM : INPUT_NOT_OVER;
        id = id == 2 ? 0 : id;
        return (id ? parse_error() : 0);
    }
    return (1);
}

