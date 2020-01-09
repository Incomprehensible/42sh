/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_codes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/12/17 17:53:52 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"
#include "sys_tools/sys_sh_configs.h"

char	get_code(void)
{
	const char	codes_buf[] = "sp&|uaqllm\\";

	if (g_parse_err >= 0 || g_syntax_err >= 0)
	{
		g_parse_err = -1;
		g_syntax_err = -1;
		return (0);
	}
	if (g_input_nover >= 0 && g_input_nover <= 10)
		return (codes_buf[g_input_nover]);
	return (0);
}

short	check_input_mir(char *str, char ch)
{
	size_t	count;
	size_t	len;

	len = ft_strlen(str);
	str += len - 1;
	count = 0;
	while (len && *str == ch)
	{
		count = count ? --count : ++count;
		--str;
		--len;
	}
	if (count)
		return (0);
	return (1);
}

short	check_input_seq(char *str, char ch)
{
	size_t	count;
	size_t	len;

	len = ft_strlen(str);
	str += --len;
	count = 0;
	while (len && *str == ch)
	{
		++count;
		--str;
		--len;
	}
	if (count != 2)
		return (0);
	return (1);
}
