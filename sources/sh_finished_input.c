/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_inputcheck.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/09/15 23:49:09 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

//WE DON'T CHECK ANYTHING INSIDE SUBSHELL BESIDES BRACKETS AND QUOTES
//WE DON'T CHECK ANYTHING INSIDE MATH BESIDES BRACKETS
//MATH SYNTAX WE WILL CHECK LATER, BEFORE PARSING

typedef struct  s_flags
{
    short sep:1;
    short ch:1;
    short tk:1;
}               t_flags;

//validate
//remove chars and insert only char start and char end
//send both brackets and quotes to validate here
size_t   count_brackets(char *str, t_dlist **tok)
{
    int     counter;
    size_t  len;

    counter = 0;
    len = 0;
    while (str[len] && !is_sep_no_space(str[len]))
    {
        if (str[len] == '\\')
            len += 2;
        else if (str[len] == '(')
            counter++;
        else if (str[len] == ')')
            counter--;
    }
    return (counter ? 0 : len);
}

short   check_seps(char *str)
{

}

short   check_word_end(char *ss, char *met, short size)
{
    if (is_token_here(ss, met))
    {
        ss += size;
        if (*ss == '\\')
            return (0);
        return (1);
    }
    return (0);
}

size_t  process_q(char *start, char *fin, char q)
{
    size_t i;

    i = 0;
    while (*start && *start != q && start != fin)
    {
        i++;
        start++;
    }
    if (!*start || !i)
        return (0);
    return (i);
}

size_t  process_qs(char *start, char *fin)
{
    while (start != fin)
    {
        //if (i)
    }
}

//в скобках мы проверяем все!! в том числе seps
size_t  process_brackets(char *str, char b, char c)
{
    size_t i;
    short n;
    char *start;
    char *fin;

    n = 0;
    while (str[n] && str[n] == b)
        ++n;
    i = n;
    start = str + n;
    while (str[i] && str[i] != c)
        i++;
    if (!str[i] || !i)
        return (0);
    fin = &str[i];
    while (str[i] == c)
    {
        i++;
        n--;
    }
    if (n)
        return (0);
    return (process_qs(start, fin));
}

size_t process_pattern(char *str, char meta)
{
    char c;
    //we alsp process quotes inside math, subshell, var, brackets etc
    //like, if we found a closing char - bracket for example - we get start and end points and we analyze quotes that didn't close inside

    c = (*str == '(') ? ')' : 0;
    c = (*str == '{') ? '}' : c;
    c = (*str == '[') ? ']' : c;
    if (meta == '"' || meta == '\'')
        return (process_q(str, str + ft_strlen(str), meta));
    return (process_brackets(str, *str, c));
}

char    *find_del(char *str)
{
    while (*str && !is_separator(*str))
        str++;
    return (str);
}

short   is_pattern(char meta)
{
    if (meta == '\'' || meta == '"' || meta == '(' || meta == '{' || meta == '[')
        return (1);
    return (0);
}

int     count_begin(char *str, char *meta, short size, int n)
{
    t_flags f;

    f.sep = 0;
    while (*str)
    {
        f.ch = 0;
        if (*str == '\\' && (f.ch = 1))
            str++;
        if (f.ch && *str)
        {
            if ((str = find_del(str + 1)))
                f.ch = 0;
        }
        else if (!f.ch && is_pattern(*str))
        {
            if (!(f.ch = process_pattern(str, *str)))
                return (-1);
            str += f.ch;
        }
        else if (!f.ch && check_word_end(str, meta, size) && (str += size))
            n++;
        str++;
    }
    return (n);
}

int     count_end(char *str, char *m1, char *m2, short size, int n)
{
    t_flags f;

    f.tk = 0;
    f.sep = 0;
    while (*str)
    {
        f.ch = 0;
        str = (*str == '\\' && (f.ch = 1) && !(f.tk = 0)) ? ++str : str;
        if (f.ch && *str)
            f.ch = ((str = find_del(str + 1))) ? 0 : f.ch;
        else if (!f.ch && is_pattern(*str))
        {
            if (!(f.ch = process_pattern(str, *str)))
                return (-1);
            str += f.ch;
        }
        else if (!f.ch && !is_token_here(str, m1) && !is_token_here(str, m2) && *str != ' ' && *str != '\t')
            f.tk = 0;
        str = (!f.ch && is_sep_no_space(*str) && (f.sep = 1)) ? ++str : str;
        n = (!f.ch && (f.sep || f.tk) && check_word_end(str, m1, size) && (str += size)) ? ++n : n;
        f.tk = (!f.ch && check_word_end(str, m2, size) && (str += ft_strlen(m2))) ? 1 : f.tk;
        str++;
    }
    return (n);
}

int   count_cycles(char *str)
{
    return (count_begin(str, "for", ft_strlen("for"), 0) + count_begin(str, "while", ft_strlen("while"), 0)
    + count_begin(str, "until", ft_strlen("until"), 0));
}

short   is_input_finished(char *str)
{
    char *point;
    //we find quote, check scripts before this quote, then make a pass if quote closes

    if ((point = ft_strstr(str, "if")))
        if (point == str || *(point - 1) != '\\')
            if ((count_begin(str, "if", ft_strlen("if"), 0) - count_end(str, "fi", "done", ft_strlen("fi"), 0)))
                return (0);
    if ((point = ft_strstr(str, "for")) || (point = ft_strstr(str, "while"))
    || (point = ft_strstr(str, "until")))
        if (point == str || *(point - 1) != '\\')
            if ((count_cycles(str) - count_end(str, "done", "fi", ft_strlen("done"), 0)))
                return (0);
    return (check_seps(str));
}


//Task 2
//для разделителей свои правила. мы смотрим на всю строку, вместе с кавычками
// && || | нужно чтоб что-то было до них и после них
// ; and & - нужно чтоб что-то было до них

//int     count_end(char *str, char *m1, char *m2, int n, short size)
//{
//    t_flags f;
//
//   while (*str)
//   {
//       if (*str == '\\' && (f.mir = 1))
//       {
//           if (!(f.space = check_space(*(++str), f.space) || !(f.sep = check_sep(*str, f.sep))))
//               f.mir = 0;
//       }
//       else
//       {
//           if (!f.mir && (f.sep || f.space) && (*str == '\'' || *str == '"'))
//           {
//                if (!(f.mir = process_q(str, *str)))
//                    return (-1);
//                str += f.mir;
//                f.mir = 0;
//           }
//           else if (!f.mir && (combos(f)) && check_word_end(f.space, str, m1, size) && (str += size))
//               n++;
//           if (!f.mir && )
//       }
//   }
//   return (n);
//}

//int     count_end(char *str, char *meta, int n)
//{
//    short mir;
//    short sep;
//    short space;
//
//    mir = 0;
//    sep = 1;
//    space = 1;
//    while (*str)
//    {
//        if (*str == '\\' && (mir = 1))
//            str = (*(str + 1) == ' ' || *(str + 1) == '\t' && !(space = 0) && !(mir = 0)) ? ++str : str;
//        while (!mir && *str && (*str == ' ' || *str == '\t') && (space = 1) && !(mir = 0))
//            str++;
//        if (!mir)
//            sep = (is_sep_no_space(*str)) ? sep : 0;
//        if (!mir && (*str == '\'' || *str == '"'))
//        {
//            if (!(mir = process_q(str, *str)))
//                return (-1);
//            str += mir;
//            mir = 0;
//        }
//        else if (!mir && sep && space && (is_token_here(str, meta)) && (str += size))
//            n++;
//        str++;
//    }
//}

//int     count_begin(char *str, char *meta, short size)
//{
//    int n;
//    short flag;
//
//    n = 0;
//    flag = 0;
//    while (*str)
//    {
//        if (*str == '\\' && (flag = 1))
//            str = ((*(str + 1) == ' ' || *(str + 1) == '\t') && (flag = 1)) ? ++str : str;
//        flag = (is_separator(*str) && flag) ? 0 : flag;
//        if (!flag && (*str == '\'' || *str == '"'))
//        {
//            if (!(flag = process_q(str, *str)))
//                return (-1);
//            str += flag;
//        }
//        else if (!flag && (is_token_here(str, meta)) && check_word_end(str, meta, size) && (str += size))
//            n++;
//        str++;
//    }
//    return (n);
//}

//If !flag && is_token_here
//        If !;
//Flag = 0;
//While space ++str
//If (chars && !is_token_here)
//If found sep flag = 1;
//система, которая чекает, что слово не экранировано
//в начале
//\ done
//в конце
//done\
//done\;
// or fi

//short   check_space(char str, short space)
//{
//    if (str == ' ' || space == '\t')
//        return (0);
//    return (space);
//}
//
//short   check_sep(char str, short sep)
//{
//    if (is_sep_no_space(str))
//        return (0);
//    return (sep);
//}

//short   combos(t_flags f)
//{
//    if ((f.space && f.tk) || (!f.tk && f.sep))
//        return (1);
//    return (0);
//}