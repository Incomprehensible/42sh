/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnancy <fnancy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:29:10 by hgranule          #+#    #+#             */
/*   Updated: 2019/11/18 22:36:55 by fnancy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYS_ERRORS_H
# define SYS_ERRORS_H

# define FATAL_BUFF_MSG_LEN 512

# define E_CMDNF 1 /* COMMAND NOR FOUND */
# define E_FRKFL 2 /* FORK FAILED */
# define E_EXECF 3 /* EXECUTE FAILED */
# define E_PIPEF 4 /* PIPE FAILED */
# define E_PERMF 5 /* PERMISSION DENIED */
# define E_REDRF 6 /* REDIRECTION FAILED */
# define E_BADFD 7 /* BAD FILE DESCRIPTOR */
# define E_ALOCF 8 /* ALLOCATION FAILS */
# define E_ISDIR 9 /* IS A DIR */
# define E_ISNRG 10 /* IS NOR REGULAR */
# define E_FCRTF 11 /* CREATION OF FILE FAILED */
# define E_FOPEF 12 /* OPEN OF FILE FAILED, DSNT EXST */
# define E_FOPPF 13 /* OPEN OF FILE FAILED, PERMS DENIED */
# define E_FOPFF 14 /* OPEN OF FILE FAILED */
# define E_NODIR 15 /* IS NOT A DIR */
# define E_NOFND 16 /* NOT FOUND*/
# define E_TMARG 17 /* TOO MANY ARGUMENTS*/
# define E_FLGER 18 /* BAD  OPTIONS */
# define E_UNFLG 19 /* 2 FLAGS */

# define ERR_CNT (E_UNFLG + 1)

/*
** Returns a standart error message string with code {ECODE}
*/
char		*sys_get_std_message(int ecode);

/*
** Exiting from a programm and puts a standart error output
*/
void		sys_fatal_memerr(char *mess);

#define ERR t_err

typedef struct      s_err
{
    int				err_code;
    char			*error_msg;
}                   t_err;

#endif
