/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_sh_configs.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 10:36:52 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/22 21:10:27 by hgranule         ###   ########.fr       */
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

# define PROMPT_OUT_FD 100

# define HIST_FILE_DENY_TRIES 16

int			g_prompt_fd;

#endif

/*
** 42sh IN FLAGS
**
** 42sh [-env f1 f2 f3 --] [-lib f1 f2 f3 --] [-logt f1] \
	[-env_fo] [-o opt=var] file/[-str <string>] param1 param2 param3
**
** -str <STRING> - If it is present, executes string (all other after string is parametrs)
** -env <file1> - specify env-init-file
** -lib <file1> <file2> <file3> -- - specify lib-files
** -logt <file1> - logout-file ????
** -env_fo - turn-off reading envp
** -o OPTION=NAME OPTION=NAME -- - options
**		OPTION:: color=yes/no
*/