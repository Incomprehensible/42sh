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
#include <stdio.h>

static t_dlist *skip_if_script(t_dlist *token_list)
{
    size_t counter;

    counter = 0;
    if (TOK_TYPE == TK_FI)
        counter++;
    token_list = token_list->prev;
    while (token_list->prev && counter)
    {
        if (TOK_TYPE == TK_FI)
            counter++;
        else if (TOK_TYPE == TK_IF)
            counter--;
        token_list = token_list->prev;
    }
    return (token_list);
}

static short   is_flow(t_tk_type type)
{
    if (type == TK_WHILE || type == TK_UNTIL || type == TK_FOR)
        return (1);
    return (0);
}

static t_dlist *skip_done_script(t_dlist *token_list)
{
    size_t counter;

    counter = 0;
    if (TOK_TYPE == TK_DONE)
        counter++;
    token_list = token_list->prev;
    while (token_list->prev && counter)
    {
        if (TOK_TYPE == TK_DONE)
            counter++;
        else if (is_flow(TOK_TYPE))
            counter--;
        token_list = token_list->prev;
    }
    return (token_list);
}

static t_dlist *skip_function(t_dlist *token_list)
{
    token_list = token_list->prev;
    while (token_list->prev && TOK_TYPE != TK_FUNCTION)
        token_list = token_list->prev;
    return (token_list);
}

static void all_to_backgr(t_dlist **token, t_dlist *backgr)
{
    t_dlist *first;

    first = token[0];
//    token[1] = backgr->prev;
//    token[1]->next = NULL;
    first->prev = backgr;
    backgr->next = first;
    backgr->prev = NULL;
    token[0] = backgr;
    token[1]->next = NULL;
//    if (first == token[1])
//        token[1]->prev = token[0];
//    else
//        first->prev = token[0];
//    if (tmp == token[1])
//        token[1]->prev = token[0];
//    else
//        tmp->prev = token[0];
//    backgr->next = token[0];
//    token[0] = backgr;
//    token[0]->prev = NULL;
}

static void to_backgr(t_dlist *token_list, t_dlist *backgr)
{
    t_dlist *tmp;

    tmp = token_list->next;
    token_list->next = backgr;
    backgr->prev = token_list;
    backgr->next = tmp;
    if (tmp && tmp->prev)
        tmp->prev = backgr;
}

static void fill_empty_hole(t_dlist *token_list, t_dlist *backgr)
{
    t_dlist *tmp_next;
    t_dlist *tmp_prev;

    tmp_next = token_list->next;
    tmp_prev = token_list->prev;
    if (TOK_VALUE)
        free(TOK_VALUE);
    free(token_list->content);
    free(token_list);
    backgr->next = tmp_next;
    backgr->prev = tmp_prev;
    tmp_prev->next = backgr;
    tmp_next->prev = backgr;
}

short   stop_point(t_tk_type type)
{
    if (type != TK_SEP && type != TK_BCKR_PS && type != HOLE)
        return (0);
    return (1);
}

t_tk_type   make_offset(t_dlist **token, t_tk_type type)
{
    t_dlist *token_list;
    t_dlist *backgr;

    make_token(token, NULL, type);
    token_list = token[1]->prev;
    backgr = token[1];
    while (token_list && !stop_point(TOK_TYPE) && token_list->prev)
    {
        if (TOK_TYPE == TK_FI)
            token_list = skip_if_script(token_list);
        else if (TOK_TYPE == TK_DONE)
            token_list = skip_done_script(token_list);
        else if (TOK_TYPE == TK_FEND)
            token_list = skip_function(token_list);
        token_list = (token_list->prev) ? token_list->prev : token_list;
    }
    token[1] = backgr->prev;
//    token[0] = backgr;
//    if (!token_list->prev)
//        all_to_backgr(token, backgr);
//    if (TOK_TYPE == HOLE)
//        fill_empty_hole(token_list, backgr);
    if (!token_list || !token_list->prev)
        all_to_backgr(token, backgr);
    else
        to_backgr(token_list, backgr);
    make_token(token, NULL, HOLE);
//    while (token[0])
//    {
//        printf("value %s type %zx\n", ((t_tok *)(token[0]->content))->value, ((t_tok *)(token[0]->content))->type);
//        token[0] = token[0]->next;
//    }
    return (TK_BCKR_PS);
}