/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlockParse.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 01:19:53 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/07 09:15:56 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/LocationBlockParse.hpp"

LocationBlockParse::LocationBlockParse()
			: _location_keyword_check(0), _location_braket_open(0),
			_location_braket_close(0), _server_braket_close(0), _location_block_count(0),
			_redirection_location_flag(0), _return_flag(0),
			_index_location_flag(0), _index_flag(0),
			_cgi_location_flag(0), _cgi_path_flag(0), _cgi_extention_flag(0),
			_redirection_location_path(""), _return(""),
			_index_location_path(""), _index(""),
			_cgi_location_path(""), _cgi_path_php(""), _cgi_extention_php(0),
			_location_parse_done(0), _config_file_name(""), _err_msg("")
{
}

LocationBlockParse::LocationBlockParse(const LocationBlockParse& locationblockparse)
			: ServerBlockParse(locationblockparse)
{
	*this = locationblockparse;
}

LocationBlockParse& LocationBlockParse::operator=(const LocationBlockParse& locationblockparse)
{
	if (this == &locationblockparse)
		return (*this);
	ServerBlockParse::operator=(locationblockparse);
	this->_location_keyword_check = locationblockparse._location_keyword_check;
	this->_location_braket_open = locationblockparse._location_braket_open;
	this->_location_braket_close = locationblockparse._location_braket_close;
	this->_server_braket_close = locationblockparse._server_braket_close;
	this->_location_block_count = locationblockparse._location_block_count;
	this->_redirection_location_flag = locationblockparse._redirection_location_flag;
	this->_return_flag = locationblockparse._return_flag;
	this->_index_location_flag = locationblockparse._index_location_flag;
	this->_index_flag = locationblockparse._index_flag;
	this->_cgi_location_flag = locationblockparse._cgi_location_flag;
	this->_cgi_path_flag = locationblockparse._cgi_path_flag;
	this->_cgi_extention_flag = locationblockparse._cgi_extention_flag;
	this->_redirection_location_path = locationblockparse._redirection_location_path;
	this->_return = locationblockparse._return;
	this->_index_location_path = locationblockparse._index_location_path;
	this->_index = locationblockparse._index;
	this->_cgi_location_path = locationblockparse._cgi_location_path;
	this->_cgi_path_php = locationblockparse._cgi_path_php;
	this->_cgi_extention_php = locationblockparse._cgi_extention_php;
	this->_location_parse_done = locationblockparse._location_parse_done;
	this->_config_file_name = locationblockparse._config_file_name;
	this->_err_msg = locationblockparse._err_msg;
	this->_redirection_location_block = locationblockparse._redirection_location_block;
	this->_index_location_block = locationblockparse._index_location_block;
	this->_cgi_location_block = locationblockparse._cgi_location_block;
	return (*this);
}

LocationBlockParse::~LocationBlockParse()
{
}

std::string	LocationBlockParse::GetRedirectionLocationPath(void) const
{
	return (this->_redirection_location_path);
}

std::string	LocationBlockParse::GetReturn(void) const
{
	return (this->_return);
}

std::string	LocationBlockParse::GetIndexLocationPath(void) const
{
	return (this->_index_location_path);
}

std::string	LocationBlockParse::GetIndex(void) const
{
	return (this->_index);
}

std::string	LocationBlockParse::GetCgiLocationPath(void) const
{
	return (this->_cgi_location_path);
}

std::string	LocationBlockParse::GetCgiPathPhp(void) const
{
	return (this->_cgi_path_php);
}

int	LocationBlockParse::GetCgiExtentionPhp(void) const
{
	return (this->_cgi_extention_php);
}

int	LocationBlockParse::GetLocationParseDone(void) const
{
	return (this->_location_parse_done);
}

int	LocationBlockParse::GetLocationBlockCount(void) const
{
	return (this->_location_block_count);
}

int	LocationBlockParse::GetServerBraketClose(void) const
{
	return (this->_server_braket_close);
}

void	LocationBlockParse::SetConfigFileName(std::string config_file_name)
{
	this->_config_file_name = config_file_name;
}

void	LocationBlockParse::LocationBlockCheck(std::string *line, int i)
{
	std::string	temp = *line;

	ServerMissedKeywordCheck(line, temp, i);

	if (temp.find("location") != std::string::npos)
		LocationKeywordLocationCheck(line, temp, i);
	else if (temp.find("{") != std::string::npos)
		LocationBraketOpenCheck(line, temp, i);
	else if (temp.find("return") != std::string::npos
		|| temp.find("index") != std::string::npos
		|| temp.find("cgi_path") != std::string::npos
		|| temp.find("cgi_extention") != std::string::npos)
	{
		if (temp.find("return") != std::string::npos)
			LocationKeywordCheck(line, temp, i, "return");
		if (temp.find("index") != std::string::npos)
			LocationKeywordCheck(line, temp, i, "index");
		if (temp.find("cgi_path") != std::string::npos)
			LocationKeywordCheck(line, temp, i, "cgi_path");
		if (temp.find("cgi_extention") != std::string::npos)
			LocationKeywordCheck(line, temp, i, "cgi_extention");
		LocationKeywordTokenCheck(line, temp, i);
	}
	else if (temp.find("}") != std::string::npos)
		LocationBraketCloseCheck(line, temp, i);
	else
	{
		if (StringCheck(temp))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_INVALID_KWD, *line, i);
			throw (this->_err_msg);
		}
	}
}

void	LocationBlockParse::ServerMissedKeywordCheck(std::string *line, std::string temp, int i)
{
	if (_location_braket_open)
		return ;

	if (ServerBlockParse::GetServerParseDone() && temp.find("location") == std::string::npos)
	{
		if (temp.find("listen") != std::string::npos)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_LISTEN_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (temp.find("host") != std::string::npos)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_HOST_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (temp.find("client_max_body_size") != std::string::npos)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_CLIENT_MAX_BODY_SIZE_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (temp.find("index") != std::string::npos)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_INDEX_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (temp.find("allow_methods") != std::string::npos)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_ALLOW_METHODS_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (temp.find("save_path") != std::string::npos)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_SAVE_PATH_EXISTS, *line, i);
			throw (this->_err_msg);
		}
	}
	if (ServerBlockParse::GetListen() == 0)
	{
		this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_LISTEN_MISSED, *line, i);
		throw (this->_err_msg);
	}
	if (ServerBlockParse::GetHost() == "")
	{
		this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_HOST_MISSED, *line, i);
		throw (this->_err_msg);
	}
	if (ServerBlockParse::GetClientMaxBodySize() == 0)
	{
		this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_CLIENT_MAX_BODY_SIZE_MISSED, *line, i);
		throw (this->_err_msg);
	}
	if (ServerBlockParse::GetIndex() == "")
	{
		this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_INDEX_MISSED, *line, i);
		throw (this->_err_msg);
	}
	if (ServerBlockParse::GetAllowMethodsFlag() == 0)
	{
		this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_ALLOW_METHODS_MISSED, *line, i);
		throw (this->_err_msg);
	}
	if (ServerBlockParse::GetSavePath() == "")
	{
		this->_err_msg = ErrMsg(this->_config_file_name, SERVER_KWD_SAVE_PATH_MISSED, *line, i);
		throw (this->_err_msg);
	}
}

void	LocationBlockParse::LocationKeywordCheck(std::string *line, std::string temp, int i, std::string keyword)
{
	if (StringCheck(temp.substr(0, temp.find(keyword)))
		|| !(temp[temp.find(keyword) + strlen(keyword.c_str())] == ' ' || temp[temp.find(keyword) + strlen(keyword.c_str())] == '\t'))
	{
		if (keyword == "location")
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_LOCATION, *line, i);
		else if (keyword == "/redirection")
		{
			if (temp[temp.find(keyword) + strlen(keyword.c_str())] == '\0')
				return ;
			if (temp[temp.find(keyword) + strlen(keyword.c_str())] == '/')
				return ;
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_REDIRECTION_LOCATION, *line, i);
		}
		else if (keyword == "/cgi-bin")
		{
			if (temp[temp.find(keyword) + strlen(keyword.c_str())] == '\0')
				return ;
			if (temp[temp.find(keyword) + strlen(keyword.c_str())] == '/')
			{
				return ;
			}
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_LOCATION, *line, i);
		}
		else if (keyword == "return")
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_RETURN, *line, i);
		
		else if (keyword == "index")
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_INDEX, *line, i);
		else if (keyword == "cgi_path")
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_PATH, *line, i);
		else if (keyword == "cgi_extention")
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_EXTENTION, *line, i);
		throw (this->_err_msg);
	}
}

void	LocationBlockParse::LocationKeywordLocationCheck(std::string *line, std::string temp, int i)
{
	std::string	token[100] = {"", };

	/* if there was location block before the new block, each block should be closed properly */
	if ((this->_location_keyword_check && this->_redirection_location_flag && this->_return_flag)
		|| (this->_location_keyword_check && this->_index_location_flag && this->_index_flag)
		|| (this->_location_keyword_check && this->_cgi_location_flag
			&& this->_cgi_path_flag && this->_cgi_extention_flag))
	{
		if (!this->_location_braket_close)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_BRAKET_CLOSE, *line, i);
			throw (this->_err_msg);
		}
	}

	/* keyword "location" exists only once in the each location block */
	LocationKeywordCheck(line, temp, i, "location");
	this->_location_keyword_check++;

	/* opening curved braket should be exists after the keyword "location" */
	if (this->_location_keyword_check == 0 && StringCheck(temp.substr(temp.find("location") + strlen("location"), std::string::npos), '{'))
	{
		this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_BRAKET_OPEN, *line, i);
		throw (this->_err_msg);
	}
	temp = temp.substr(temp.find("location") + strlen("location"), std::string::npos);

	if (temp.find("/") != std::string::npos)
	{
		TokenCount(temp, token);
		if (token[0].find("{") != std::string::npos)
			token[0] = token[0].substr(0, token[0].find("{"));
		if (token[0].find("/redirection") != std::string::npos)
		{
			/* checking the number of slash at the end of redirection location path */
			if (token[0].substr(token[0].find("/redirection") + strlen("/redirection"), std::string::npos).find("/") != std::string::npos)
			{
				if (token[0].substr(token[0].find("/redirection") + strlen("/redirection"), std::string::npos).substr(token[0].substr(token[0].find("/redirection") + strlen("/redirection"), std::string::npos).find("/") + strlen("/"), std::string::npos).find("/") != std::string::npos)
				{
					this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_REDIRECTION_LOCATION, *line, i);
					throw (this->_err_msg);
				}
			}
			if (!CheckDirectory(HttpBlockParse::GetRoot() + token[0], 0))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_REDIRECTION_LOCATION_PATH, *line, i);
				throw (this->_err_msg);
			}
			this->_redirection_location_flag++;
			this->_redirection_location_path = token[0];
		}
		else if (token[0].find("/cgi-bin") != std::string::npos)
		{
			if (this->_cgi_location_flag)
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_LOCATION_EXISTS, *line, i);
				throw (this->_err_msg);
			}

			/* checking the number of slash at the end of cgi-bin location path */
			if (token[0].substr(token[0].find("/cgi-bin") + strlen("/cgi-bin"), std::string::npos).find("/") != std::string::npos)
			{
				if (token[0].substr(token[0].find("/cgi-bin") + strlen("/cgi-bin"), std::string::npos).substr(token[0].substr(token[0].find("/cgi-bin") + strlen("/cgi-bin"), std::string::npos).find("/") + strlen("/"), std::string::npos).find("/") != std::string::npos)
				{
					this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_REDIRECTION_LOCATION, *line, i);
					throw (this->_err_msg);
				}
			}
			if (!CheckDirectory(HttpBlockParse::GetRoot() + token[0], CGI_BIN_LOCATION))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_CGI_LOCATION_PATH, *line, i);
				throw (this->_err_msg);
			}
			this->_cgi_location_flag++;
			this->_cgi_location_path = token[0];
		}
		else if (token[0].length() > 1 && token[0][0] == '/')
		{
			/* checking the number of slash at the end of index location path */
			if (token[0].substr(token[0].find("/") + strlen("/"), std::string::npos).find("/") != std::string::npos)
			{
				if (token[0].substr(token[0].find("/") + strlen("/"), std::string::npos).substr(token[0].substr(token[0].find("/") + strlen("/"), std::string::npos).find("/") + strlen("/"), std::string::npos).find("/") != std::string::npos)
				{
					this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_INDEX_LOCATION, *line, i);
					throw (this->_err_msg);
				}
			}
			if (!CheckDirectory(HttpBlockParse::GetRoot() + token[0], 0))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_INDEX_LOCATION_PATH, *line, i);
				throw (this->_err_msg);
			}
			this->_index_location_flag++;
			this->_index_location_path = token[0];
		}
		else
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_LOCATION, *line, i);
			throw (this->_err_msg);
		}
	}

	/* keyword "location" always needs a path after */
	if (!(this->_redirection_location_flag || this->_index_location_flag || this->_cgi_location_flag))
	{
		this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_LOCATION, *line, i);
		throw (this->_err_msg);
	}
	if (temp.find("{") != std::string::npos)
	{
		if (this->_redirection_location_flag || this->_index_location_flag || this->_cgi_location_flag)
			temp = temp.substr(temp.find(token[0]) + strlen(token[0].c_str()), std::string::npos);
		if (StringCheck(temp.substr(0, temp.find("{"))))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_INVALID_KWD, *line, i);
			throw (this->_err_msg);
		}
		LocationBraketOpenCheck(line, temp, i);
	}
}

void	LocationBlockParse::LocationBraketOpenCheck(std::string *line, std::string temp, int i)
{
	this->_location_braket_open++;
	if (temp.find("{") != 0 && !StringCheck(temp.substr(0, temp.find("{"))))
		temp = temp.substr(temp.find("{"), std::string::npos);

	/* opening curved braket exists only once in the each location block */
	if (temp.substr(temp.find("{") + strlen("{"), std::string::npos).find("{") != std::string::npos)
	{
		this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_BRAKET_OPEN, *line, i);
		throw (this->_err_msg);
	}
	if (temp.find("return") != std::string::npos
		|| temp.find("index") != std::string::npos
		|| temp.find("cgi_path") != std::string::npos
		|| temp.find("cgi_extention") != std::string::npos)
	{
		temp = temp.substr(temp.find("{") + 1, std::string::npos);
		
		if (temp.find("return")!= std::string::npos)
			LocationKeywordCheck(line, temp, i, "return");
		if (temp.find("index") != std::string::npos)
			LocationKeywordCheck(line, temp, i, "index");
		if (temp.find("cgi_path") != std::string::npos)
			LocationKeywordCheck(line, temp, i, "cgi_path");
		if (temp.find("cgi_extention") != std::string::npos)
			LocationKeywordCheck(line, temp, i, "cgi_extention");
		LocationKeywordTokenCheck(line, temp, i);
	}

	/* in case of redirection location block or index location block have finished in one line
		: already have done parseing and initing the location block */
	if ((this->_location_keyword_check == 0 && (temp.find("return") != std::string::npos || temp.find("index") != std::string::npos)))
		return ;

	if (temp.substr(temp.find("{") + strlen("{"), std::string::npos).find("}") != std::string::npos)
	{
		/* redirection location block and index location block can be finished in one line */
		if (!((this->_redirection_location_flag && this->_return_flag)
			|| (this->_index_location_flag && this->_index_flag)))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_BRAKET_OPEN, *line, i);
			throw (this->_err_msg);
		}
	}
}

void	LocationBlockParse::LocationKeywordTokenCheck(std::string *line, std::string temp, int i)
{
	std::string	token[100] = {"", };

	if (temp.find("return") != std::string::npos)
	{
		if (!this->_redirection_location_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_RETURN, *line, i);
			throw (this->_err_msg);
		}
		if (!this->_location_braket_open)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_BRAKET_OPEN, *line, i);
			throw (this->_err_msg);
		}
		if (this->_return_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_RETURN_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (TokenCount(2, temp, token) && !this->_location_parse_done)
		{
			if (!((TokenCount(temp, token) == 3 && token[1].find(";") == std::string::npos && token[2].find(";") != std::string::npos)
				|| (TokenCount(temp, token) == 3 && token[1].find(";") != std::string::npos && token[2].find(";") == std::string::npos)
				|| (TokenCount(temp, token) == 4 && token[1].find(";") == std::string::npos && token[2].find(";") != std::string::npos)))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_RETURN, *line, i);
				throw (this->_err_msg);
			}
		}
	}
	else if (temp.find("index") != std::string::npos)
	{
		if (!this->_index_location_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_INDEX, *line, i);
			throw (this->_err_msg);
		}
		if (!this->_location_braket_open)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_BRAKET_OPEN, *line, i);
			throw (this->_err_msg);
		}
		if (this->_index_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_INDEX_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (TokenCount(2, temp, token) && !this->_location_parse_done)
		{
			if (!((TokenCount(temp, token) == 3 && token[1].find(";") == std::string::npos && token[2].find(";") != std::string::npos)
				|| (TokenCount(temp, token) == 3 && token[1].find(";") != std::string::npos && token[2].find(";") == std::string::npos)
				|| (TokenCount(temp, token) == 4 && token[1].find(";") == std::string::npos && token[2].find(";") != std::string::npos)))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_INDEX, *line, i);
				throw (this->_err_msg);
			}
		}
	}
	else if (temp.find("cgi_path") != std::string::npos)
	{
		if (!this->_cgi_location_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_PATH, *line, i);
			throw (this->_err_msg);
		}
		if (!this->_location_braket_open)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_BRAKET_OPEN, *line, i);
			throw (this->_err_msg);
		}
		if (this->_cgi_path_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_PATH_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (TokenCount(2, temp, token) && !this->_location_parse_done)
		{
			if (!(TokenCount(temp, token) == 3 && token[1].find(";") == std::string::npos))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_PATH, *line, i);
				throw (this->_err_msg);
			}
		}
	}
	else if (temp.find("cgi_extention") != std::string::npos)
	{
		if (!this->_cgi_location_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_EXTENTION, *line, i);
			throw (this->_err_msg);
		}
		if (!this->_location_braket_open)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_BRAKET_OPEN, *line, i);
			throw (this->_err_msg);
		}
		if (this->_cgi_extention_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_EXTENTION_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (TokenCount(2, temp, token) && !this->_location_parse_done)
		{
			if (!(TokenCount(temp, token) == 3 && token[1].find(";") == std::string::npos))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_EXTENTION, *line, i);
				throw (this->_err_msg);
			}
		}
	}
	LocationBlockGetInfo(token, line, temp, i);
}

void	LocationBlockParse::LocationBlockGetInfo(std::string *token, std::string *line, std::string temp, int i)
{
	if (token[0] == "return" && !this->_location_parse_done)
	{
		if (SemicolonCheck(token[1]))
		{
			if (!((token[2] != "" && token[1].find(";") == std::string::npos && token[2].find(";") != std::string::npos && !SemicolonCheck(token[2]))
				|| (token[2] == "}" && !SemicolonCheck(token[1]))
				|| (token[1].find(";") == std::string::npos && (token[2] == ";" || token[2] == ";}"))))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_SEMICOLON, *line, i);
				throw (this->_err_msg);
			}
		}
		if (token[1].find(";") != std::string::npos)
			token[1] = token[1].substr(0, token[1].find(";"));
		this->_return_flag++;
		this->_return = token[1];
	}
	if (token[0] == "index" && !this->_location_parse_done)
	{
		if (SemicolonCheck(token[1]))
		{
			if (!((token[2] != "" && token[1].find(";") == std::string::npos && token[2].find(";") != std::string::npos && !SemicolonCheck(token[2]))
				|| (token[2] == "}" && !SemicolonCheck(token[1]))
				|| (token[1].find(";") == std::string::npos && (token[2] == ";" || token[2] == ";}"))))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_SEMICOLON, *line, i);
				throw (this->_err_msg);
			}
		}
		if (token[1].find(";") != std::string::npos)
			token[1] = token[1].substr(0, token[1].find(";"));
		if (!(token[1].find(".html") != std::string::npos
			&& token[1].find(".html") == token[1].length() - 5)
			|| token[1].substr(token[1].find(".") + strlen("."), std::string::npos).find(".") != std::string::npos)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_INDEX_VALUE, *line, i);
			throw (this->_err_msg);
		}
		this->_index_flag++;
		this->_index = token[1];
	}
	if (token[0] == "cgi_path" && !this->_location_parse_done)
	{
		if (SemicolonCheck(token[1]))
		{
			if (!(token[2] != "" && token[1].find(";") == std::string::npos && !SemicolonCheck(token[2])))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_SEMICOLON, *line, i);
				throw (this->_err_msg);
			}
		}
		if (token[2] == "")
		{
			token[1] = token[1].substr(0, token[1].find(";"));
			token[1] = RemoveSpaceTab(token[1]);
		}
		if (!(token[1] == "/usr/bin/php-cgi"))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_PATH, *line, i);
			throw (this->_err_msg);
		}
		this->_cgi_path_php = token[1];
		this->_cgi_extention_php = 1;
		this->_cgi_path_flag++;
	}
	if (token[0] == "cgi_extention" && !this->_location_parse_done)
	{
		if (SemicolonCheck(token[1]))
		{
			if (!(token[2] != "" && token[1].find(";") == std::string::npos && !SemicolonCheck(token[2])))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_SEMICOLON, *line, i);
				throw (this->_err_msg);
			}
		}
		if (token[2] == "")
		{
			token[1] = token[1].substr(0, token[1].find(";"));
			token[1] = RemoveSpaceTab(token[1]);
		}
		if (!(token[1] == ".php"))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_EXTENTION, *line, i);
			throw (this->_err_msg);
		}
		this->_cgi_extention_php = 1;
		this->_cgi_extention_flag++;
	}
	
	/* when redirection location block or index location block finishes in one line */
	if ((temp.find("return") != std::string::npos && temp.find("}") != std::string::npos)
		|| (temp.find("index") != std::string::npos && temp.find("}") != std::string::npos))
	{
		temp = temp.substr(temp.find(";") + strlen(";"), std::string::npos);
		LocationBraketCloseCheck(line, temp, i);
	}
}

void	LocationBlockParse::LocationMissedKeywordCheck(std::string *line, int i)
{
	if (this->_redirection_location_flag && !this->_return_flag)
	{
		this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_RETURN_MISSED, *line, i);
		throw (this->_err_msg);
	}
	if (this->_index_location_flag && !this->_index_flag)
	{
		this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_INDEX_MISSED, *line, i);
		throw (this->_err_msg);
	}
	if (this->_cgi_location_flag)
	{
		if (!this->_cgi_path_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_PATH_MISSED, *line, i);
			throw (this->_err_msg);
		}
		else if (!this->_cgi_extention_flag)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_EXTENTION_MISSED, *line, i);
			throw (this->_err_msg);
		}
	}
}

void	LocationBlockParse::LocationBraketCloseCheck(std::string *line, std::string temp, int i)
{
	/* for location block closing */
	if (_location_braket_open == _location_braket_close + 1 && temp.find("}") != std::string::npos)
	{
		if (StringCheck(temp, '}'))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_INVALID_KWD, *line, i);
			throw (this->_err_msg);
		}
		
		/* check missed keyword in the location block */
		if (!((this->_redirection_location_flag && this->_return_flag)
			|| (this->_index_location_flag && this->_index_flag)
			|| (this->_cgi_location_flag && this->_cgi_path_flag && this->_cgi_extention_flag)))
		{
			LocationMissedKeywordCheck(line, i);
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_BRAKET_CLOSE, *line, i);
			throw (this->_err_msg);
		}

		_location_braket_close++;
		_location_block_count++;
		ServerBlockParse::IncreaseTotalLocationBlock();

		/* location block init: only redirection location block and index location block element */
		if ((this->_redirection_location_flag && this->_return_flag)
			|| (this->_index_location_flag && this->_index_flag)
			|| (this->_cgi_location_flag && this->_cgi_path_flag && this->_cgi_extention_flag))
		{
			SaveLocationBlock();
			this->_location_keyword_check = 0;
			this->_redirection_location_flag = 0;
			this->_return_flag = 0;
			this->_index_location_flag = 0;
			this->_index_flag = 0;
			this->_redirection_location_path = "";
			this->_return = "";
			this->_index_location_path = "";
			this->_index = "";
		}
		temp = temp.substr(temp.find("}") + strlen("}"), std::string::npos);
		if (StringCheck(temp, '}'))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_INVALID_KWD, *line, i);
			throw (this->_err_msg);
		}
	}

	/* for server block closing */
	if (_location_braket_open == _location_braket_close && !_location_parse_done && temp.find("}") != std::string::npos)
	{
		if (StringCheck(temp, '}'))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, SERVER_INVALID_KWD, *line, i);
			throw (this->_err_msg);
		}
		if (this->_location_keyword_check)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, SERVER_BRAKET_CLOSE, *line, i);
			throw (this->_err_msg);
		}
		_server_braket_close++;
		_location_parse_done++;
		if (_location_parse_done && _cgi_location_flag == 0)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, LOCATION_KWD_CGI_LOCATION_MISSED, *line, i);
			throw (this->_err_msg);
		}
		temp = temp.substr(temp.find("}") + strlen("}"), std::string::npos);
		if (StringCheck(temp, '}'))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, SERVER_INVALID_KWD, *line, i);
			throw (this->_err_msg);
		}
	}

	/* for http block closing */
	if (_server_braket_close && _location_parse_done && temp.find("}") != std::string::npos)
	{
		if (StringCheck(temp, '}'))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_INVALID_KWD, *line, i);
			throw (this->_err_msg);
		}
		ServerBlockParse::SetHttpBraketCloseSaveHttp();
		temp = temp.substr(temp.find("}") + strlen("}"), std::string::npos);
		if (StringCheck(temp))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_INVALID_KWD, *line, i);
			throw (this->_err_msg);
		}
	}

	/* server block init, location block init */
	if (_location_braket_close && _server_braket_close)
	{
		ServerBlockParse::SaveServerBlock(this->_redirection_location_block, this->_index_location_block, this->_cgi_location_block);
		ServerBlockParse::InitServerBlockParseData();
		InitLocationBlockParseData();
		ServerBlockParse::SetLocationBlockEnded(1);
	}
}

void	LocationBlockParse::SaveLocationBlock(void)
{
	if ((this->_redirection_location_flag && this->_return_flag))
	{
		t_redirection	save;

		save.path = this->_redirection_location_path;
		save.return_value = this->_return;

		this->_redirection_location_block.push_back(save);
	}
	if ((this->_index_location_flag && this->_index_flag))
	{
		t_index		save;

		save.path = this->_index_location_path;
		save.index = this->_index;

		this->_index_location_block.push_back(save);
	}
	if ((this->_cgi_location_flag && this->_cgi_path_flag && this->_cgi_extention_flag))
	{
		t_cgi		save;

		save.path = this->_cgi_location_path;
		save.cgi_path = this->_cgi_path_php;
		save.cgi_extention = this->_cgi_extention_php;

		this->_cgi_location_block.push_back(save);
	}
}

void	LocationBlockParse::InitLocationBlockParseData(void)
{
	this->_location_keyword_check = 0;
	this->_location_braket_open = 0;
	this->_location_braket_close = 0;
	this->_server_braket_close = 0;
	this->_location_block_count = 0;
	this->_redirection_location_flag = 0;
	this->_return_flag = 0;
	this->_index_location_flag = 0;
	this->_index_flag = 0;
	this->_cgi_location_flag = 0;
	this->_cgi_path_flag = 0;
	this->_cgi_extention_flag = 0;
	this->_redirection_location_path = "";
	this->_return = "";
	this->_index_location_path = "";
	this->_index = "";
	this->_cgi_location_path = "";
	this->_cgi_path_php = "";
	this->_cgi_extention_php = 0;
	this->_location_parse_done = 0;
	this->_err_msg = "";
	while (this->_redirection_location_block.size())
		_redirection_location_block.pop_back();
	while (this->_index_location_block.size())
		_index_location_block.pop_back();
	while (this->_cgi_location_block.size())
		_cgi_location_block.pop_back();
}
