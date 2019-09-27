/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_sh_configs.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 10:36:52 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/27 14:43:00 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_SH_CONFIGS_H
# define SYS_SH_CONFIGS_H

# define SHELL_NAME_STD "42sh"
# define SH_VAR_SHNAME "SHELL_NAME"
# define SH_VAR_HIST_RC_PATH "HOME"

# define SH_RC_FILE ".42shrc"
# define SH_HIST_FILE ".42history"

# define SH_VAR_PROMPT "PS"
# define SH_PROMPT ">>> "
# define SH_PROMPT_QT "qt> "
# define SH_PROMPT_AP "ap> "
# define SH_PROMPT_HD "heredoc> "
# define SH_PROMPT_SBH "subsh> "
# define SH_PROMPT_LM "lambda> "
# define SH_PROMPT_PIP "pipe> "

# define HIST_FILE_DENY_TRIES 16;

int			history_file_status;
int			rc_file_status;

#endif
