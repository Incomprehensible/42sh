/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_codes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/30 06:58:43 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "sys_tools/sys_sh_configs.h"

char	get_code(void)
{
	const char	codes_buf[] = "sp&|uaqll";

	if (parse_err >= 0 || syntax_err >= 0)
	{
		parse_err = -1;
		syntax_err = -1;
		return (0);
	}
	if (input_not_over >= 0 && input_not_over <= 8)
		return (codes_buf[input_not_over]);
	return (0);
}
