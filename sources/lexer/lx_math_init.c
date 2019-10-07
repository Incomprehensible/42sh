/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_math_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/07 03:45:08 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

t_graph  *empty_in(void)
{
    static t_graph *math;

    if (math)
        return (math);
    math = (t_graph *)malloc((sizeof(t_graph)));
    math->type = TK_EMPTY;
    math->patt = NULL;
    math->forward = NULL;
    math->left = NULL;
    math->right = NULL;
    math->next = NULL;
    return (math);
}

t_graph  *sep_in(void)
{
    static t_graph *math;

    if (math)
        return (math);
    math = (t_graph *)malloc((sizeof(t_graph)));
    math->type = TK_SEP;
    math->patt = NULL;
    math->forward = NULL;
    math->left = NULL;
    math->right = NULL;
    math->next = NULL;
    return (math);
}

t_graph  *close_in(char *del, t_tk_type type)
{
    static t_graph *math;

    math = (t_graph *)malloc((sizeof(t_graph)));
    math->type = type;
    math->patt = del;
    math->forward = sep_in();
    if (type == APOF_END)
        math->left = empty_in();
    else
        math->left = NULL;
    math->right = NULL;
    math->next = NULL;
    return (math);
}

t_graph  *operator_in(t_graph *operand)
{
    static t_graph *math;

    math = (t_graph *)malloc((sizeof(t_graph)));
    math->type = OPERATOR;
    math->patt = NULL;
    math->forward = operand;
    math->left = NULL;
    math->right = NULL;
    math->next = NULL;
    return (math);
}

t_graph  *operand_in(short ind)
{
    t_graph *math;

    math = (t_graph *)malloc((sizeof(t_graph)));
    math->type = OPERAND;
    math->patt = NULL;
    if (!ind)
    {
        math->forward = operator_in(math);
        math->left = close_in("'", APOF_END);
    }
    else
    {
        math->forward = operator_in(math);
        math->left = close_in("))", BRACKET_END);
    }
    math->right = NULL;
    math->next = NULL;
    return (math);
}

//here we can have as many numbers and operator as we want
//rules - we must not have only operators
// and the last one must be operand, not sign
// signs can be the first ever
//but as many proceeding numbers and signs as we want - check screen
t_graph  *operation_in()
{
    t_graph *math;

    math = (t_graph *)malloc((sizeof(t_graph)));
    math->type = MATH_SEQ;
    math->patt = NULL;
    math->forward = sep_in();
    math->left = empty_in();
    math->right = NULL;
    math->next = NULL;
    return (math);
}

t_graph  *let_math_in(void)
{
    static t_graph *math;

    if (math)
        return (math);
    math = (t_graph *)malloc((sizeof(t_graph)));
    math->type = APOF;
    math->patt = "'";
    math->forward = operand_in(0);
    math->left = operator_in(math->forward->forward);
    math->right = NULL;
    //second branch
    math->next = operation_in();
    return (math);
}

t_graph  *brackets_math_in(void)
{
    static t_graph *math;

    if (math)
        return (math);
    math = (t_graph *)malloc((sizeof(t_graph)));
    math->type = BRACKET;
    math->patt = "((";
    math->forward = operand_in(1);
    math->left = operator_in(math->forward->forward);
    math->right = NULL;
    math->next = NULL;
    return (math);
}
