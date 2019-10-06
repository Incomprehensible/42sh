/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_hidden.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 18:13:30 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/07 01:18:49 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_HIDDEN_H
# define MATH_HIDDEN_H

# include "parser.h"
# include "bltn.h"
# include "ft_ctype.h"

long			math_eval(DSTRING *expr, ENV *envr);

size_t			op_search(DSTRING *expr, char *op, size_t op_len);
int				m_skip_brackets(char *str, size_t *ind);
void			skip_word(char *str, size_t *ind);
void			skip_alnums_n_space(char *str, size_t *ind);
void			m_skip_spaces(char *str, size_t *ind);

DSTRING			*get_res_var(long res);
char			*get_name_var(char *str_xp);
long			expr_ret(DSTRING *expr, ENV *envr);
long			value_ret(char *str, ENV *envr);

long			dec_inc_do(DSTRING *ex, ENV *envr, int is_dec);
long			mop_inc(DSTRING *ex, ENV *envr);
long			mop_dec(DSTRING *ex, ENV *envr);

long			mop_neq(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_eq(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_and(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_or(DSTRING *ex1, DSTRING *ex2, ENV *envr);

long			mop_lw(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_bg(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_lweq(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_bgeq(DSTRING *ex1, DSTRING *ex2, ENV *envr);

long			mop_mod(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_suj(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_div(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_mlt(DSTRING *ex1, DSTRING *ex2, ENV *envr);
long			mop_add(DSTRING *ex1, DSTRING *ex2, ENV *envr);

#endif
