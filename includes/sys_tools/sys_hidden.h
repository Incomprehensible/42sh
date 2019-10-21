/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_hidden.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 08:56:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/10/21 15:37:47 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_HIDDEN_H
# define SYS_HIDDEN_H

# include "sys_tools/sys_sh_configs.h"

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include "ft_mem.h"
# include "ft_dlist.h"

# define SYS_FDS_USR_LIMIT 60
# define SYS_PIPES_SIZE 1024
# define SYS_PRGS_SIZE 1024

# define SYS_REDR_OFFST 80

# define CHECK_INTS_P 0x0000008000000080

# define SYS_TCH_F 0x8
# define SYS_TCH_W 0x4
# define SYS_TCH_R 0x2
# define SYS_TCH_X 0x1

# define SYS_TCH_TYP 0xF0

# define SYS_TCH_PIP 0x10
# define SYS_TCH_CHR 0x20
# define SYS_TCH_DIR 0x40
# define SYS_TCH_BLK 0x60
# define SYS_TCH_REG 0x80
# define SYS_TCH_LNK 0xA0
# define SYS_TCH_SOC 0xC0
# define SYS_TCH_WHT 0xE0

# define PS_M_FG 1
# define PS_M_BG 2

# define PS_S_RUN 4
# define PS_S_STP 3
# define PS_S_SIG 2
# define PS_S_DON 1

typedef struct	s_proc_grp
{
	t_dlist			*members;
	char			*input_line;
	pid_t			pgid;
	int				mode;
	int				state;
	int				signal;
}					t_pgrp;

typedef struct		s_proc_ds
{
	pid_t			pid;
	unsigned char	state;
	unsigned char	exit_st;
	char			signal;
}					t_ps_d;

t_pgrp				*p_table[SYS_PRGS_SIZE];
char				sys_pipes[SYS_PIPES_SIZE];
pid_t				hot_gid;
pid_t				hot_sbsh;
char				*hot_bkgr;
int					g_jid;
int					g_intr;

#endif
