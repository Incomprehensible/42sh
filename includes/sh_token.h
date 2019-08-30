/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdaemoni <gdaemoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:29:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/30 00:21:04 by gdaemoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TOKEN_H
# define SH_TOKEN_H

typedef enum		e_tk_type
{
	empty, // Empty token
	cmd, // cmd or arg
	sep, // ;
	pipes, // pipe |
	eof // end of file input
}					t_tk_type;

typedef struct		s_token
{
	char			*value;
	t_tk_type		type;
}					t_tok;

#endif
