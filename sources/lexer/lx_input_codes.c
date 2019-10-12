/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_codes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/10/12 14:23:15 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "sys_tools/sys_sh_configs.h"

char    get_code(void)
{
	const char 	codes_buf[] = "sp&|uaql";

    // if (!codes_buf[0])
    // {
    //     codes_buf[PRO_SUBSH] =  SH_PROMPT_SBH;
    //     codes_buf[PRO_PIPE] =  SH_PROMPT_PIP;
    //     codes_buf[PRO_AND] =  SH_PROMPT_AND;
    //     codes_buf[PRO_OR] =  SH_PROMPT_OR;
    //     codes_buf[PRO_NONE] =  SH_PROMPT;
    //     codes_buf[PRO_APOF] =  SH_PROMPT_AP;
    //     codes_buf[PRO_DQUOT] =  SH_PROMPT_QT;
    //     codes_buf[PRO_LAM] =  SH_PROMPT_LM;
    // }
    if (PARSE_ERR >= 0 || SYNTAX_ERR >= 0)
    {
        PARSE_ERR = -1;
        SYNTAX_ERR = -1;
        return (0);
    }
    if (INPUT_NOT_OVER >= 0 && INPUT_NOT_OVER < 8)
        return (codes_buf[INPUT_NOT_OVER]);
    return (0);
}