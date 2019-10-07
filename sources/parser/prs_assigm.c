/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_assigm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:01:26 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/07 12:40:05 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char		*prs_assigm_val_get(t_dlist **tks, ENV *envs)
{
	t_dlist		*tki;
	char		*val;

	tki = *tks;
	tki = arg_sub(tki, &val, 0, envs);
	*tks = tki;
	return (val);
}

static int		prs_assigm_do(char *key, char *n_val, char asgm_tp, ENV *envs)
{
	DSTRING		*val_buff;
	DSTRING		*orig_val;

	val_buff = dstr_new(n_val);
	if (asgm_tp == '+')
	{
		if (!(orig_val = env_get_variable(key, envs)))
			return (-1);
		if ((dstr_insert_dstr(val_buff, orig_val, 0)) < 0)
			return (-1);
		dstr_del(&orig_val);
	}
	if (env_set_variable(key, val_buff, envs) < 0)
		return (-1);
	dstr_del(&val_buff);
	return (0);
}

t_dlist			*prs_assigm(t_dlist *tks, ENV *envs, int *status)
{
	char		*var_val[2];
	char		assigm_type;
	t_tok		*tok;

	if (tks && (tok = tks->content))
		var_val[0] = (tok->type == TK_VAR) ? tok->value : 0;
	else
		return (0);
	if (tks->next && (tok = tks->next->content) && tok->type == TK_ASSIGM)
		assigm_type = tok->value[0];
	else
		return (0);
	tks = tks->next;
	tks = tks->next;
	var_val[1] = prs_assigm_val_get(&tks, envs);
	*status = prs_assigm_do(var_val[0], var_val[1], assigm_type, envs);
	if (var_val[1])
		free(var_val[1]);
	return (tks);
}
