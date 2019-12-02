/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_codes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/03 00:58:38 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "sys_tools/sys_sh_configs.h"

char	get_code(void)
{
	const char	codes_buf[] = "sp&|uaqllm";

	if (g_parse_err >= 0 || g_syntax_err >= 0)
	{
		g_parse_err = -1;
		g_syntax_err = -1;
		return (0);
	}
	if (g_input_nover >= 0 && g_input_nover <= 9)
		return (codes_buf[g_input_nover]);
	return (0);
}
