/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rms.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 15:56:29 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/27 15:12:59 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RMS_H
# define RMS_H

# include "ft_dlist.h"
# include "ft_avl_tree.h"
# include "executer.h"
# include "parser.h"
# include <stdlib.h>

/*
** FREES A REDIRECTION FROM parser.h
*/
void		et_rm_rdr(void *redir, size_t type);

/*
** FREES A CHAR** NULL TERMINATED ARRAY
*/
void		et_rm_warr(char **args);

/*
** FREES A EXPRESSION FROM parser.h
*/
void		et_rm_expr(EXPRESSION *expr);

/*
** FREES A CONTENT OF ETAB * from executer.h
*/
void		et_rm_ett(void *et_cont, size_t type);

void		free_token(void *tok, size_t sz);

#endif