/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_hidden.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 08:56:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/18 22:37:08 by fnancy           ###   ########.fr       */
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

/* IS IT EXIST */
# define SYS_TCH_F 0x8
/* IS IT WRITEABLE */
# define SYS_TCH_W 0x4
/* IS IT READABLE */
# define SYS_TCH_R 0x2
/* IS IT EXECUTABLE */
# define SYS_TCH_X 0x1

/* MASK FOR GETTING FILE TYPE */
# define SYS_TCH_TYP 0xF0

/* IS IT FIFO */
# define SYS_TCH_PIP 0x10
/* IS IT CHAR SPECIAL */
# define SYS_TCH_CHR 0x20
/* IS IT DIRECTORY */
# define SYS_TCH_DIR 0x40
/* IS IT BLOCK SPECIAL */
# define SYS_TCH_BLK 0x60
/* IS IT REGULAR */
# define SYS_TCH_REG 0x80
/* IS IT SYMBOL LINK */
# define SYS_TCH_LNK 0xA0
/* IS IT SOCKET */
# define SYS_TCH_SOC 0xC0
/* IS IT WHITEOUT */
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
	int				flag;
}					t_pgrp;

typedef struct		s_proc_ds
{
	pid_t			pid;
	unsigned char	state;
	unsigned char	exit_st;
	char			signal;
}					t_ps_d;

t_pgrp				*g_ptab[SYS_PRGS_SIZE];
char				g_pipes[SYS_PIPES_SIZE];
pid_t				hot_gid;
pid_t				g_hsh;
char				*g_hbg;
int					g_jid;
int					g_intr;

int			sys_wait_prg(t_pgrp **ps_grp, int *status, pid_t lpid, int mode);
int			sys_del_if_done(ssize_t i, int state_code);

#endif
