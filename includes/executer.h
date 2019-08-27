/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 09:09:58 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/27 01:18:30 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_H
# define EXECUTER_H

# include <stdlib.h>

# define ETAB t_extab

# define ET_EXPR (size_t)1
# define ET_PIPE (size_t)2

typedef struct		s_extab			// RELOADING OF T_DLIST
{
	void			*instruction;
	size_t			type;
	struct s_extab	*next_e;
	struct s_extab	*prev_e;
}					t_extab;

# include "parser.h"

/*
** BLTN FUNC
** Functions executes command with COMMAND_DESCRIPTOR.
** It creates pipe if OUTPUT_PIPE pointer is not 0, and close INPUT_PIPES
** in parrent proc after all;
** Creates redirections following the redir_descriptor;
** Returns a child's pid.
*/
int			exe_execute_pi(EXPRESSION *cmd, ENV *vars);
void		exe_redir_ex(REDIRECT *rdr);
int			exe_wait_cps(pid_t);
int			exe_execute_expr(EXPRESSION *expr, ENV *envr, int *status);

void		exe_redir_save420(t_dlist *redrs);
void		exe_redir_load420(t_dlist *redrs);
int			exe_execute_b(EXPRESSION *expr, ENV *envr, \
int (*bltn)(char **, ENV *), int *status);
int			exe_execute_pi_b(EXPRESSION *expr, ENV *envr, \
int (*bltn)(char **, ENV *));

#endif
