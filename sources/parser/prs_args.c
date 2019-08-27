/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 03:17:54 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/27 18:21:05 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "sh_token.h"

int				prs_deref_name(t_dlist **tks, ENV *envs)
{
	t_tok		*dtok;
	t_avln		*v_node; //! VAR TMP
	t_dlist		*tmp;
	char		*valu;

	tmp = (*tks)->next;
	ft_dlst_delcut(tks, free_token);
	(*tks) = tmp;
	dtok = tmp->content;
	dtok->type = value_tk;
	//! GET VARIABLE
	if (!(v_node = ft_avl_search(envs->globals, dtok->value)))
		valu = "";
	else
		valu = v_node->content;
	free(dtok->value);
	if (!(dtok->value = ft_strdup(valu)))
		return (-1); //!MALLOC ERRROR -1 code
	return (0);
}

int				prs_vars_derefs(t_dlist **tks, ENV *envs)
{
	t_tok		*tok;
	t_dlist		*it;

	it = *tks;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type == deref_tk)
		{
			tok = it->next->content;
			if (tok->type == name_tk && (prs_deref_name(&it, envs) < 0))
				return (-1); // ! MALLOC ERROR code -1
		}
		it = it->next;
	}
	return (0);
}

int				prs_join_exprs(t_dlist **tks)
{
	char		*valu[2];
	char		*newval;
	t_dlist		*tki;
	size_t		len;

	valu[0] = ((t_tok *)(*tks)->content)->value;
	valu[1] = ((t_tok *)(*tks)->next->content)->value;
	if (!(newval = ft_strjoin(valu[0], valu[1])))
		return(-1); //! MALLOC ERROR code -1
	tki = (*tks)->next;
	ft_dlst_delcut(tks, free_token);
	free(valu[1]);
	((t_tok *)tki->content)->value = newval;
	((t_tok *)tki->content)->type = expr_tk;
	if (!(len = ft_strlen(((t_tok *)tki->content)->value)))
		ft_dlst_delcut(&tki, free_token);
	return (0);
}

int				prs_values_joins(t_dlist **tks)
{
	t_tok		*tok;
	t_dlist		*it;
	size_t		len;

	it = *tks;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		if (tok->type == expr_tk || tok->type == value_tk)
		{
			tok = it->next->content;
			if (tok->type == value_tk || tok->type == expr_tk)
				prs_join_exprs(&it); //TODO! NEED TO CONTROL MALLOC ERROR !!!!
			else if (((t_tok *)it->content)->type == value_tk)
			{
				tok = it->content;
				if ((len = ft_strlen(tok->value)) == 0)
					ft_dlst_delcut(&it, free_token);
				else
					tok->type = expr_tk;
			}
		}
		it = it->next;
	}
	return (0);
}

char			**prs_args(t_dlist **tokens, ENV *envs)
{
	char		**args;
	size_t		argslen;
	size_t		i;
	t_dlist		*it;
	t_tok		*tok;

	argslen = 0;
	prs_vars_derefs(tokens, envs); //! MALLOC CHECK NEED
	prs_values_joins(tokens); //! MALLOC CHECK NEED
	it = *tokens;
	while (it && prs_is_a_instruction((tok = it->content)))
	{
		tok->type == expr_tk ? ++argslen : 0;
		it = it->next;
	}
	if (!(args = ft_memalloc(sizeof(char*) * (argslen + 1))))
		return (0); //!MALLOC CALL FAILED. (SU.)
	it = *tokens;
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
