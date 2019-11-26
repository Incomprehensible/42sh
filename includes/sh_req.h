/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_req.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:34:45 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/26 19:54:11 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_REQ_H
# define SH_REQ_H

# include "libft.h"
# include "ft_avl_tree.h"
# include "env.h"
# include "sh_options.h"

int			sys_init(int sbh);
int			sys_var_init(ENV *env);
int			sh_launch_system(ENV *env, char *flags, char **argv);
int			sh_launch_file(t_opt *opt, ENV *env);
int			sys_core_set_init(ENV *env, char **argv);
void		sh_loop(ENV *env);

int			sh_do_src(char *filename, ENV *env);
int			sh_do_str(ENV *env, char *str);

#endif
