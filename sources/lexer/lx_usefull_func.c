/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_usefull_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/08 22:27:58 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

short   check_valid_sep(t_dlist *token_list)
{
    if (!token_list->content || !TOK_TYPE)
        return (0);
    while (token_list && TOK_TYPE != TK_SEP)
        token_list = token_list->prev;
    token_list = (token_list) ? token_list->prev : token_list;
    while (token_list && TOK_TYPE == TK_EMPTY)
        token_list = token_list->prev;
    if (TOK_TYPE != TK_EXPR && TOK_TYPE != TK_NAME && TOK_TYPE != TK_VALUE && 
        TOK_TYPE != TK_SUBSH && TOK_TYPE != TK_MATH && !is_tok_redir(TOK_TYPE, 0))
        return (0);
    return (1);
}

short   sep_detected(t_dlist *token_list)
{
    if (!token_list->content || !TOK_TYPE)
        return (0);
    while (token_list && TOK_TYPE == TK_EMPTY)
        token_list = token_list->prev;
    if (!token_list)
        return (0);
    if (TOK_TYPE != TK_SEP)
        return (0);
    return (1);
}

// short   sep_detected(t_dlist *token_list, short sep)
// {
//     if (!token_list->content || !TOK_TYPE)
//         return (0);
//     while (token_list && TOK_TYPE == TK_EMPTY)
//         token_list = token_list->prev;
//     if (!token_list)
//         return (0);
//     if (TOK_TYPE != TK_SEP)
//         return (0);
//     if (sep && sep != '\n' && ft_strcmp(TOK_VALUE, ";"))
//         return (0);
//     return (1);
// }

char    *skip_spaces(char *str)
{
    while (*str && (*str == ' ' || *str == '\t'))
        str++;
    return (str);
}

short   graph_end(t_graph *g, char *str)
{
    while (*str && (*str == ' ' || *str == '\t'))
        str++;
    if (g->type == TK_DONE || g->type == TK_FI)
        if (is_token_here(skip_spaces(str), "done") ||
        is_token_here(skip_spaces(str), "fi"))
            return (1);
    if ((g->type == TK_FI || g->type == TK_DONE) && !(*str))
        return (1);
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

size_t get_seq(char *str)
{
    size_t i;

    i = 0;
    while (*str >= 48 && *str <= 57)
    {
        str++;
        i++;
    }
    return (i);
}
