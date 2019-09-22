/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_hidden.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 08:56:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/22 11:18:22 by hgranule         ###   ########.fr       */
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

# define SYS_FDS_USR_LIMIT 40
# define SYS_PIPES_SIZE 1024

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

char		sys_pipes[SYS_PIPES_SIZE];

#endif
