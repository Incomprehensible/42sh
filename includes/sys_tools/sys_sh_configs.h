/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_sh_configs.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bomanyte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 10:36:52 by hgranule          #+#    #+#             */
/*   Updated: 2019/12/01 18:26:37 by bomanyte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_SH_CONFIGS_H
# define SYS_SH_CONFIGS_H

# define SHELL_NAME_STD "42sh"
# define SH_VAR_SHNAME "SHELL_NAME"
# define SH_VAR_HIST_RC_PATH "HOME"

# define PROMPT_MAX_INFO_BUFF (1024 * 9)
# define PROMPT_OK_STATUS_C "\033[32mOK\033[0m"
# define PROMPT_KO_STATUS_C "\033[31mKO\033[0m"

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
# define SH_PROMPT_AND "and> "
# define SH_PROMPT_OR "or> "
# define SH_PROMPT_MATH "math> "

# define PROMPT_OUT_FD 100

# define HIST_FILE_DENY_TRIES 16

int			g_prompt_fd;

#endif
