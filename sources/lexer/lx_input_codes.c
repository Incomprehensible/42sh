/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_codes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/27 12:29:25 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "sys_tools/sys_sh_configs.h"

char    get_code(void)
{
	const char 	codes_buf[] = "sp&|uaqll";

    if (PARSE_ERR >= 0 || SYNTAX_ERR >= 0)
    {
        PARSE_ERR = -1;
        SYNTAX_ERR = -1;
        return (0);
    }
    if (INPUT_NOT_OVER >= 0 && INPUT_NOT_OVER <= 8)
        return (codes_buf[INPUT_NOT_OVER]);
    return (0);
}