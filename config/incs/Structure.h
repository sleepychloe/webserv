/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 17:18:01 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/04 18:53:15 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H

#include <string>
#include <vector>

typedef struct	s_rediection
{
	std::string			path;
	std::string			return_value;
}	t_redirection;

typedef struct	s_index
{
	std::string			path;
	std::string			index;
}	t_index;

typedef struct s_cgi
{
	std::string			path;
	std::string			cgi_path;
	int				cgi_extention;
}	t_cgi;

typedef struct s_server_block
{
	int				listen;
	std::string			host;
	int				client_max_body_size;
	std::string			index;
	int				allow_methods_get;
	int				allow_methods_post;
	int				allow_methods_delete;
	std::string			save_path;
	std::vector<t_redirection>	redirection_location_block;
	std::vector<t_index>		index_location_block;
	std::vector<t_cgi>		cgi_location_block;
}	t_server_block;

typedef struct s_http_blcok
{
	std::string			root;
	std::string			autoindex;
	std::string			default_error_page;
	std::string			err_page_directory;
	std::vector<t_server_block>	server_block;
}	t_http_block;

#endif
