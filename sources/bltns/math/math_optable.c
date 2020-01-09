/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_optable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/24 23:56:30 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

void	compare_level_in(t_tk_type *arr)
{
	arr[8 * 2 + 0] = IS_EQU;
	arr[8 * 2 + 1] = NO_EQU;
	arr[8 * 2 + 2] = LESS_EQ;
	arr[8 * 2 + 3] = MORE_EQ;
	arr[8 * 2 + 4] = LESS;
	arr[8 * 2 + 5] = MORE;
	arr[8 * 2 + 6] = 0;
}

void	normal_level_in(t_tk_type *arr)
{
	arr[8 * 1 + 0] = PLUS;
	arr[8 * 1 + 1] = MINUS;
	arr[8 * 1 + 2] = MULT;
	arr[8 * 1 + 3] = DEVID;
	arr[8 * 1 + 4] = DEVREM;
	arr[8 * 1 + 5] = 0;
}

void	bit_level_in(t_tk_type *arr)
{
	arr[8 * 0 + 0] = BIT_R;
	arr[8 * 0 + 1] = BIT_L;
	arr[8 * 0 + 2] = AND;
	arr[8 * 0 + 3] = OR;
	arr[8 * 0 + 4] = XOR;
	arr[8 * 0 + 5] = 0;
}

void	logic_level_in(t_tk_type *arr)
{
	arr[8 * 3 + 0] = LOG_AND;
	arr[8 * 3 + 1] = LOG_OR;
	arr[8 * 3 + 2] = 0;
}

void	assign_level_in(t_tk_type *arr)
{
	arr[8 * 4 + 0] = EQU;
	arr[8 * 4 + 1] = PLUS_EQ;
	arr[8 * 4 + 2] = MIN_EQ;
	arr[8 * 4 + 3] = 0;
}
