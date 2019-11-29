/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_script_in_4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 00:53:18 by bomanyte          #+#    #+#             */
/*   Updated: 2019/11/23 21:49:13 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh_req.h"
#include "sh_token.h"
#include "sh_tokenizer.h"

static t_graph	*expr_init(t_graph *tk)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_EXPR;
	script->patt = "~;";
	script->forward = done_init();
	script->left = tk;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

t_graph			*do_init(t_graph *tk)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = TK_DO;
	script->patt = "do_";
	script->forward = expr_init(tk);
	script->left = tk;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

t_graph			*condition_init(t_graph *whiletk)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	script->type = EX;
	script->patt = "~;";
	script->forward = do_init(whiletk);
	script->left = NULL;
	script->right = NULL;
	script->next = NULL;
	return (script);
}

static void		whiletk_init(t_graph *script)
{
	script->type = TK_WHILE;
	script->patt = "while_";
	script->forward = forexpr_init(script, TK_WHILE);
	script->left = condition_init(script);
	script->right = NULL;
	script->next = NULL;
}

t_graph			*while_script_in(void)
{
	t_graph *script;

	script = (t_graph *)malloc((sizeof(t_graph)));
	whiletk_init(script);
	return (script);
}
