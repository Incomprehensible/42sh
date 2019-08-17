/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_vars.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/15 13:00:33 by fnancy            #+#    #+#             */
/*   Updated: 2019/08/17 16:04:04 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_VARS_H
#define SH_VARS_H
# define S_DARR_STRINGS		2096
# define S_ENVP_FILES		1024
# include <sys/types.h>
# include <dirent.h>
# include "libft.h"
# include "ft_avl_tree.h"
# include "dstring.h"


/* FLAGS 
	cmd -c ["string"] - do string
	shr -r  - litlle interpretator*/
typedef struct	s_flags
{
	char		cmd;
	char		sht;
}				t_flags;


/* SH_VAR
	global - global vars from parent process
	local  - local vars
	flgs   - flags, when 42sh startet
	files  - files, when 42sh startet
	flg_c_string - string for flag c(if he has) */
typedef	struct	s_envp
{
	t_avl_tree	*global;
	t_avl_tree	*local;
	t_flags		flgs;
	DSTRING		*files[S_ENVP_FILES];
	DSTRING		*flg_c_string;
}				t_envp;

/*	strings - array of DSTRING'S(every string is name of command / directory)
	maxlen  - max length name command / directory
	count   - count elements in array
	allsize - count symbols all elements in array */
typedef struct	s_darr
{
	DSTRING		*strings[S_DARR_STRINGS];
	size_t 		maxlen;
	size_t 		count;
	size_t 		allsize;
}				t_darr;


/* INIT GLOBAL AND LOCAL ENV
	argc - count of arguments from main()
	argv - arguments from main()
	envp - envp from main()
	env  - adreess for env*/
void		sh_init_vars(int argc, char **argv, char **envp, t_envp *env);

/*	GET ALL CMDS FROM $PATH */
t_darr		get_list_cmds(t_envp *envp);

/* PARSE ARGV */
int			sh_parser_argv(int argc, char **argv, t_envp *env);

#endif