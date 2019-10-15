/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_dbg.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 19:14:06 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/15 19:30:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_DBG_H
# define SYS_DBG_H

#include "sys_tools/sys_errors.h"
#include "sys_tools/sys_sh_configs.h"
#include "sys_tools/sys_tools.h"

int			DBG_SYS_GLB(void);
int			DBG_SYS_SNAP(void);
void		DBG_PRINT_TOKENS(t_dlist *toklst);
void		DBG_SYS_CONFS(void);

int			bltn_dbg_snap(char **args, ENV *envr);

int			dbg_tok_pr_flag;

#endif
