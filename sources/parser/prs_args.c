/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:17:54 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/25 03:18:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"

char			**prs_args(t_dlist *tokens)
{
	char		**args;
	size_t		argslen;
	size_t		i;
	t_dlist		*it;
	t_tok		*tok;

	argslen = 0;
	it = tokens;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		tok->type == expr_tk ? ++argslen : 0;
		it = it->next;
	}
	if (!(args = ft_memalloc(sizeof(char*) * (argslen + 1))))
		return (0); //!MALLOC CALL FAILED. (SU.)
	it = tokens;
	i = -1;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type == expr_tk)
			if (!(args[++i] = ft_strdup(tok->value)))
				return (0); //! MALLOC CALL FAILED. (NOT SAFE, MAY BE LEAKS).
		it = it->next;
	}
	return (args);
}
