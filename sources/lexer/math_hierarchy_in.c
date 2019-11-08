/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_hierarchy_in.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/07 03:38:44 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

void	first_level_in(t_tk_type *arr)
{
	arr[0] = INCRM;
	arr[1] = DECRM;
	arr[2] = NEGAT;
	arr[3] = REJECT;
	arr[4] = NOT;
	arr[5] = POSIT;
	arr[6] = 0;
}

void	second_level_in(t_tk_type *arr)
{
	arr[0] = BIT_L;
	arr[1] = BIT_R;
	arr[2] = 0;
}

void	third_level_in(t_tk_type *arr)
{
	arr[0] = AND;
	arr[1] = OR;
	arr[2] = XOR;
	arr[3] = 0;
}

void	forth_level_in(t_tk_type *arr)
{
	arr[0] = MULT;
	arr[1] = DEVID;
	arr[2] = DEVREM;
	arr[3] = 0;
}

void	fivth_level_in(t_tk_type *arr)
{
	arr[0] = PLUS;
	arr[1] = MINUS;
	arr[2] = 0;
}

void	sixth_level_in(t_tk_type *arr)
{
	arr[0] = EQU;
	arr[1] = PLUS_EQ;
	arr[2] = MIN_EQ;
	arr[3] = IS_EQU;
	arr[4] = NO_EQU;
	arr[5] = MORE_EQ;
	arr[6] = LESS_EQ;
	arr[7] = MORE;
	arr[8] = LESS;
	arr[9] = 0;
}

void	seventh_level_in(t_tk_type *arr)
{
	arr[0] = LOG_AND;
	arr[1] = LOG_OR;
	arr[2] = 0;
}

void	ops_init(t_tk_type **ops)
{
	first_level_in(ops[0]);
	second_level_in(ops[1]);
	third_level_in(ops[2]);
	forth_level_in(ops[3]);
	fivth_level_in(ops[4]);
	sixth_level_in(ops[5]);
	seventh_level_in(ops[6]);
}