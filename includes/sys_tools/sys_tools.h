/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_tools.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 08:55:44 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/27 15:19:14 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_TOOLS_H
# define SYS_TOOLS_H

# include <unistd.h>
# include <signal.h>
# include "env.h"

# include "sys_tools/sys_sh_configs.h"

int			sys_create_pipe(int	*a);
int			sys_destroy_pipe(int p);
int			sys_kill_pipes(void);

int			sys_wait_cp(pid_t childs_pid, int *status);
int			sys_wait_cps(pid_t last_child, int *status);

/*
** Puts a string {MESSAGE} with length {LEN} to the standart
** error output. If {LEN} == 0, its calculates length by itself.
*/
int			sys_error_message(char *message, size_t len);

/*
** Puts a standart message for this error code {ECODE}, and
** adds a special message {DESCR}. Returns -1 * {ECODE}.
*/
int			sys_error_handler(char *descr, int ecode, ENV *envr);

int			sys_file_op(const char *path, ENV *envr, \
const int flags, char *er_context);

int			sys_getch(void);

/*
** FUnction for file testing.
** Returns {PERMISSIONS} which consist of bit-flags.
** [ TYPE - FRWX ]
** TYPE - bits for a file type. (man lstat / check header defines).
** F - is file exists.
** R - is file readable.
** W - is file writeable.
** X - is file executable.
** 
** EXAMPLES:
** 1) EXECUTABLE REGULAR FILE : [ 1000 1**1 ]
** 2) DIRECTORY FILE : [ 0100 1*** ]
** 3) REGULAR FILE without rwx permissions : [ 1000 1000 ]
*/
int			sys_touch_file(const char *path);

/*
** Returns 0 if file is executable and regular
** Returns -1 if it is not exist
** Returns -2 if it is a directory
** Returns -3 if it is not executable
** Returns -4 if anything else
*/
int			sys_is_ex_bin(const char *path);

/*
** Returns a new string with path to the file called {FILE} at 
** standart configuration directory.
**
** FATAL ERROR IS MALLOC FAIL.
** DUPLICATES A NEW STRING!
*/
char		*sys_get_conf_path(char *file, ENV *envr);

/*
** Returns a DYN_string with STD_PROMPT
** If type is 0, returns PS varibale's value. 
** Else if type is 'p', '\\', 'l', 'h', 's', 'a', 'q'
** returns a one of const values.
**
** FATAL ERROR IS MALLOC FAIL.
** CREATES A NEW DSTRING!
*/
DSTRING		*sys_get_prompt_num(ENV *envr, char type);
int			sys_put_prompt(ENV *envr, char type);

int			sys_write_history(DSTRING *line, ENV *envr);

#endif
