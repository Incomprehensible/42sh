/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_input_check.c                                   :+:      :+:    :+:   */
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

char	*qa_ap_cls_n(char *str, int *a)
{
	while (*str)
	{
		if (*str == '\'' && (--(*a) || 1))
			return (++str);
		if (*str++ == '\\' && *str == '\'')
			++str;
	}
	return (str);
}

char	*qa_cls_n(char *str, int *a, int *q)
{
	while (*str)
	{
		if (*str == '\\' && ++str)
			++str;
		else if (*str == '\'' && (++(*a) || 1))
			str = qa_ap_cls_n(++str, a);
		else if (*str == '\"' && (++(*q) || 1))
			str = qa_qt_cls_n(++str, q);
		else
			++str;
	}
	return (str);
}

short	quotes_closed(char *str)
{
	int		aps;
	int		qts;

	aps = 0;
	qts = 0;
	qa_cls_n(str, &aps, &qts);
	if (aps)
	{
		g_input_nover = PRO_APOF;
		return (0);
	}
	if (qts)
	{
		g_input_nover = PRO_DQUOT;
		return (0);
	}
	return (1);
}

int		validate_cycles(char *str, char *meta)
{
	int	num;

	num = 0;
	while (*str)
	{
		if ((*str == '\n' || *str == ';') && ++str)
			str = cycle_to_the_start(str);
		if (*str == '\\')
			str += mirror_passes(str);
		else if (is_it_q(*str) || is_it_br(*str))
		{
			str += skip_field(str, *str);
			++str;
		}
		else
		{
			if (are_tokens_here(str))
				num++;
			else if (is_token_here(str, meta))
				num--;
			str++;
		}
	}
	return (num);
}

short	scripts_closed(char *str)
{
	char	*tmp;

	str = skip_spaces(str);
	tmp = str;
	tmp = if_to_the_start(str);
	if (*tmp && validate_ifs(tmp) > 0)
		return (0);
	str = cycle_to_the_start(str);
	if (*str && validate_cycles(str, "done") > 0)
		return (0);
	return (1);
}
