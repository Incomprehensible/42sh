/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_hidden.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 08:56:09 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/20 20:39:44 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_HIDDEN_H
# define SYS_HIDDEN_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
// # ifdef __linux__
// #  include <sys/wait.h>
// # endif
# include "ft_mem.h"

# define SYS_FDS_USR_LIMIT 40
# define SYS_PIPES_SIZE 1024

# define CHECK_INTS_P 0x0000008000000080

# define SHELL_NAME_STD "42sh"

char		sys_pipes[SYS_PIPES_SIZE];

#endif
