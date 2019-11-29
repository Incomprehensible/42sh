/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_get_deref.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 09:14:22 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/29 16:53:37 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executer.h"
#include "sh_token.h"
#include "ft_lbuffer.h"
#include "sys_tools/sys_tools.h"

char			*get_deref_name(char *key, ENV *envr)
{
	t_dyn_string	*dstr;
	char			*res;

	if (!(dstr = env_get_variable(key, envr)))
		return (0);
	res = ft_strdup(dstr->txt);
	dstr_del(&dstr);
	if (!res)
		return (0);
	return (res);
}

char			*get_deref_math(char *expr, ENV *envr)
{
	long		num;
	char		*res;
	ERR			err;
	int			i;

	i = 0;
	err.error_msg = NULL;
	err.err_code = 0;
	num = do_math_bltn(expr, envr, &err);
	if (err.err_code && math_error_handler(&err, 1, envr))
		return (NULL);
	res = ft_lltoa_base(num, 10);
	return (res);
}

char			*get_deref(t_dlist *tokens, ENV *envr)
{
	const t_tok		*tok = tokens->content;

	if (tok->type == TK_NAME)
		return (get_deref_name(tok->value, envr));
	if (tok->type == TK_MATH)
		return (get_deref_math(tok->value, envr));
	if (tok->type == TK_SUBSH)
		return (get_deref_subsh(tok->value, envr));
	return (0);
}
