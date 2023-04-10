/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlockParse.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 00:07:16 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/04 18:21:53 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_BLOCK_PARSE_HPP
# define SERVER_BLOCK_PARSE_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "./HttpBlockParse.hpp"
#include "./Structure.h"
#include "./ParseUtils.h"
#include "./DefineErrMsg.h"

class	ServerBlockParse: public HttpBlockParse
{
public:
	ServerBlockParse();
	ServerBlockParse(const ServerBlockParse& serverblockparse);
	ServerBlockParse& operator=(const ServerBlockParse& serverblockparse);
	virtual ~ServerBlockParse();

	int			GetListen(void) const;
	std::string		GetHost(void) const;
	int			GetClientMaxBodySize(void) const;
	std::string		GetIndex(void) const;
	int			GetAllowMethodsGet(void) const;
	int			GetAllowMethodsPost(void) const;
	int			GetAllowMethodsDelete(void) const;
	int			GetAllowMethodsFlag(void) const;
	std::string		GetSavePath(void) const;

	int			GetServerParseDone(void) const;
	int			GetServerBlockCount(void) const;
	int			GetTotalLocationBlock(void) const;
	int			GetHttpBraketClose(void) const;

	void			SetConfigFileName(std::string config_file_name);
	void			SetLocationBlockEnded(int i);
	void			SetHttpBraketCloseSaveHttp(void);

	void			IncreaseTotalLocationBlock(void);

	void			SaveServerBlock(std::vector<t_redirection> redirection_location,
					std::vector<t_index> index_location, std::vector<t_cgi> cgi_location);
	void			InitServerBlockParseData(void);

	void			ServerBlockCheck(std::string *line, int i);

private:
	void			HttpMissedKeywordCheck(std::string *line, std::string temp, int i);

	void			ServerKeywordCheck(std::string *line, std::string temp, int i, std::string keyword);
	void			ServerKeywordServerCheck(std::string *line, std::string temp, int i);
	void			ServerBraketOpenCheck(std::string *line, std::string temp, int i);
	void			ServerKeywordTokenCheck(std::string *line, std::string temp, int i);
	void			ServerBlockGetInfo(std::string *token, std::string *line, int i);

	int			_server_keyword_check;
	int			_server_braket_open;
	int			_server_block_count;

	int			_listen_flag;
	int			_host_flag;
	int			_client_max_body_size_flag;
	int			_index_flag;
	int			_allow_methods_flag;
	int			_save_path_flag;

	int			_listen;
	std::string		_host;
	int			_client_max_body_size;
	std::string		_index;
	int			_allow_methods_get;
	int			_allow_methods_post;
	int			_allow_methods_delete;
	std::string		_save_path;

	int			_server_parse_done;
	std::string		_config_file_name;
	std::string		_err_msg;

	int			_total_location_block;
	int			_location_block_ended;
	int			_http_braket_close;

	std::vector<t_server_block>	_server_block;
};

#endif
