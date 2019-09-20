/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_errors.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 20:29:10 by hgranule          #+#    #+#             */
/*   Updated: 2019/09/20 23:06:51 by hgranule         ###   ########.fr       */
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

# define ERR_CNT (E_ISNRG + 1)

char		*sys_get_std_message(int ecode);

#endif
