/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_req.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:34:45 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/27 22:47:08 by hgranule         ###   ########.fr       */
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
int			sh_launch_file(t_opt *opt, ENV *env);
int			sys_core_set_init(ENV *env, char **argv);
void		sh_loop(ENV *env);
int			sh_launch_loop(ENV *env);
int			sh_launch(t_opt *opt, ENV *env);

int			sh_do_src(char *filename, ENV *env);
int			sh_do_str(ENV *env, char *str);
size_t		ft_dlst_len(t_dlist *lst);
char		**ft_dlst_to_warr(t_dlist *lst, \
			char *(*cnt_cb)(void *, size_t sz));
char		*callback_dlst_warr(void *cnt, size_t sz);

#endif
