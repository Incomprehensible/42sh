/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/12 01:29:37 by hgranule          #+#    #+#             */
/*   Updated: 2019/08/22 12:18:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_TOKEN_H
# define SH_TOKEN_H

typedef enum		e_tk_type
{
	empty_tk, //* empty token for separations of args, or something else.
	expr_tk, //* EXPRESSION token for cmds, args, builtns or something like that.
	sep_tk, //* ';' - separation token.
	or_tk, //* '||' - or operator token.
	and_tk, //* '&&' - and operator token.
	bckr_ps_tk, //* '&' - background procces token.
	rd_w_tk, //* '> >&' redirection token for write.
	rd_r_tk, //* '< <&' redirection token for read.
	rd_a_tk, //* '>>' redirection token for appending-write.
	rd_rw_tk, //* '<>' redirection token for read_n_write.
	hered_tk, word_tk, //* '<< WORD' here-document tokens.
	fd_tk, filename_tk, //* file_descriptor token and file_name token for redirections.
	pipe_tk, //* pipe token.
	assigm_tk, //* '= +=' assigment operator token for setting variables.
	name_tk, value_tk, //* '[varname] = [value]', tokens for vars.
	math_tk, //* '(()), let' arephmetic operator token.
	subsh_tk, //* '()' sub-shell token.
	deref_tk, //* '$' dereferencing token.
	if_tk, then_tk, else_tk, fi_tk, while_tk, do_tk, done_tk, for_tk, in_tk, until_tk, //* flow_instructions tokens.
	exec_tk, function_tk, //* exec and function tokens.
	apof_tk, dquote_tk, //* [ ', " ] - double-quotes and single-quotes tokens.
	eof_tk, //* end_of_file(input) token.
	fend_tk, //* end of the function body.
	var_tk //* - '{}' variable engine token.
}					t_tk_type;

typedef struct		s_token
{
	char			*value;
	t_tk_type		type;
}					t_tok;

#endif
