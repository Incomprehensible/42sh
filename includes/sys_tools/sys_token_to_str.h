/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_token_to_str.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 09:30:04 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/20 05:53:38 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_TOKEN_TO_STR_H
# define SYS_TOKEN_TO_STR_H

# include "dstring.h"
# include "sh_token.h"
# include "ft_dlist.h"
# include "executer.h"

# define TOKSTR_LMT 72
# define TOKSTR_FAIL "<FAIL TO GET LINE>"

DSTRING		*tok_get_dstring(t_tok *tok);
DSTRING		*token_get_dvalue(t_tok *tok, int rd_fl);
char		*str_from_n_tkls(t_dlist *toks, size_t tlen, \
t_dlist *end_tk, t_tk_type end_t_tk);
char		*sys_get_str_from_etab(ETAB **etab);

#endif
