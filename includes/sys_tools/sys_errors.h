/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hgranule@21-school.ru>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:29:10 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/27 21:00:21 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_ERRORS_H
# define SYS_ERRORS_H

# define FATAL_BUFF_MSG_LEN 512

/*
** COMMAND NOR FOUND
*/
# define E_CMDNF 1

/*
** FORK FAILED
*/
# define E_FRKFL 2

/*
** EXECUTE FAILED
*/
# define E_EXECF 3

/*
** PIPE FAILED
*/
# define E_PIPEF 4

/*
** PERMISSION DENIED
*/
# define E_PERMF 5

/*
** REDIRECTION FAILED
*/
# define E_REDRF 6

/*
** BAD FILE DESCRIPTOR
*/
# define E_BADFD 7

/*
** ALLOCATION FAILS
*/
# define E_ALOCF 8

/*
** IS A DIR
*/
# define E_ISDIR 9

/*
** IS NOR REGULAR
*/
# define E_ISNRG 10

/*
** CREATION OF FILE FAILED
*/
# define E_FCRTF 11

/*
** OPEN OF FILE FAILED, DSNT EXST
*/
# define E_FOPEF 12

/*
** OPEN OF FILE FAILED, PERMS DENIED
*/
# define E_FOPPF 13

/*
** OPEN OF FILE FAILED
*/
# define E_FOPFF 14

/*
** IS NOT A DIR
*/
# define E_NODIR 15

/*
** NOT FOUND
*/
# define E_NOFND 16

/*
** TOO MANY ARGUMENTS
*/
# define E_TMARG 17

/*
** BAD OPTIONS
*/
# define E_FLGER 18

/*
** 2 FLAGS
*/
# define E_UNFLG 19

/*
** zero length
*/
# define E_ARGZL 42

# define ERR_CNT (E_UNFLG + 1)

/*
** Returns a standart error message string with code {ECODE}
*/
char				*sys_get_std_message(int ecode);

/*
** Exiting from a programm and puts a standart error output
*/
void				sys_fatal_memerr(char *mess);

# define ERR t_err

typedef struct		s_err
{
	int				err_code;
	char			*error_msg;
}					t_err;

# define MSG_ME_VLTG "invalid operand: value too great for base"
# define MSG_ME_IVOP "syntax error: invalid operator"
# define MSG_ME_DCMP "syntax error: double compare"
# define MSG_ME_OPEX "syntax error: operand expected"
# define MSG_ME_IVAS "error: attempted assignment to non-variable"
# define MSG_ME_STRO "error: string operand"
# define MSG_ME_DVZR "error: division by zero"
# define MSG_ME_WERR "error: undefiend error (allocation or smt)"

#endif
