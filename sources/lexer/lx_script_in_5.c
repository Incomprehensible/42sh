/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_script_in_5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/23 21:50:06 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

t_graph	*sep_init(void)
{
	static t_graph *script;

	if (script)
		return (script);
	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_SEP;
	script->patt = NULL;
	script->forward = NULL;
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

t_graph	*forexpr_init(t_graph *fortk, t_tk_type tk)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	if (tk == TK_FOR)
		script->type = FORMATH;
	else
		script->type = MATH_NT;
	script->patt = NULL;
	script->forward = do_init(fortk);
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

t_graph	*done_init(void)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_DONE;
	script->patt = "done_";
	script->forward = sep_init();
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}
