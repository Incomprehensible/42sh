/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:29:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/17 12:24:51 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TOKEN_H
# define SH_TOKEN_H

typedef enum		e_tk_type
{
	empty_tk, // Empty token
	cmd_tk, // cmd or arg
	rdr_tk, // redirection read
	rdw_tk, // redirection write
	rda_tk, // redirection append
	rdrw_tk, // redirection rw
	fd_tk, // file descriptor
	filename_tk, // file name
	sep_tk, // ;
	pipe_tk, // pipe |
	eof_tk // end of file input
}					t_tk_type;

typedef struct		s_token
{
	char			*value;
	t_tk_type		type;
}					t_tok;

#endif
