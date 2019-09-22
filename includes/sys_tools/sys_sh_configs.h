/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_sh_configs.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 10:36:52 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/22 11:19:45 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_SH_CONFIGS_H
# define SYS_SH_CONFIGS_H

# define SHELL_NAME_STD "42sh"
# define SH_VAR_SHNAME "SHELL_NAME"
# define SH_VAR_HIST_RC_PATH "HOME"

# define SH_RC_FILE ".42shrc"
# define SH_HIST_FILE ".42history"

int			history_file_status;
int			rc_file_status;

#endif
