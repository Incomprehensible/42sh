/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_bases_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/09 05:28:03 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "bltn_math/math_hidden.h"

t_mtx    *init_binary(void)
{
	t_mtx *base;

    base = (t_mtx *)malloc(sizeof(t_mtx));
	base->strt_meta = ft_strdup("0x~");
    base->fin_meta = ft_strdup("0x@01@");
    base->base = BIN;
	base->next = NULL;
    return (base);
}

t_mtx    *init_hex(void)
{
	t_mtx *base;

    base = (t_mtx *)malloc(sizeof(t_mtx));
	base->strt_meta = ft_strdup("0x~");
    base->fin_meta = ft_strdup("0x@0123456789abcdefABCDEF@");
    base->base = HEX;
	base->next = NULL;
    return (base);
}

t_mtx    *init_seven(void)
{
	t_mtx	*base;
	t_mtx	*strt;

    base = (t_mtx *)malloc(sizeof(t_mtx));
	strt = base;
	base->strt_meta = ft_strdup("0o~");
	base->fin_meta = ft_strdup("0o@01234567@");
    base->base = SEV;
	base->next = (t_mtx *)malloc(sizeof(t_mtx));
	base = base->next;
	base->strt_meta = ft_strdup("0~");
    base->fin_meta = ft_strdup("0@01234567@");
	base->base = SEV;
	base->next = NULL;
    return (strt);
}

t_mtx    *init_decimal(void)
{
	t_mtx *base;

    base = (t_mtx *)malloc(sizeof(t_mtx));
	base->strt_meta = ft_strdup("@0123456789@");
    base->fin_meta = base->strt_meta;
    base->base = DEC;
	base->next = NULL;
    return (base);
}

void	init_num_bases(t_mtx **bases)
{
	bases[0] = init_binary();
	bases[1] = init_hex();
	bases[2] = init_seven();
	bases[3] = init_decimal();
	bases[4] = NULL;
}