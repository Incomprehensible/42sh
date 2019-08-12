/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:29:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/12 12:06:27 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TOKEN_H
# define SH_TOKEN_H

typedef enum		e_tk_type
{
	empty, // Empty token
	cmd, // cmd or arg
	sep, // ;
	pip, // pipe |
	eof // end of file input
}					t_tk_type;

typedef struct		s_token
{
	char			*value;
	t_tk_type		type;
}					t_tok;

#endif
