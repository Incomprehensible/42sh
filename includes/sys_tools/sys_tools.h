/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_tools.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 08:55:44 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/22 17:53:50 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_TOOLS_H
# define SYS_TOOLS_H

# include "env.h"
# include <unistd.h>
# include <signal.h>

# include "sys_tools/sys_sh_configs.h"

int			sys_create_pipe(int	*a);
int			sys_destroy_pipe(int p);
int			sys_kill_pipes(void);

int			sys_wait_cp(pid_t childs_pid, int *status);
int			sys_wait_cps(pid_t last_child, int *status);

int			sys_error_message(char *message, size_t len);
int			sys_error_handler(char *descr, int ecode, ENV *envr);

int			sys_getch(void);
int			sys_touch_file(const char *path);
int			sys_is_ex_bin(const char *path);

#endif
