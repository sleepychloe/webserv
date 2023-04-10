/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpBlockParse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 23:52:58 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/07 18:28:43 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/HttpBlockParse.hpp"

HttpBlockParse::HttpBlockParse()
			: _http_keyword_check(0), _http_braket_open(0), _http_block_count(0),
			_root_flag(0), _autoindex_flag(0), _default_err_page_flag(0),
			_root(""), _autoindex(""), _default_err_page(""), _err_page_directory(""),
			_http_parse_done(0), _config_file_name(""), _err_msg("")
{
}

HttpBlockParse::HttpBlockParse(const HttpBlockParse& httpblockparse)
{
	*this = httpblockparse;
}

HttpBlockParse& HttpBlockParse::operator=(const HttpBlockParse& httpblockparse)
{
	if (this == &httpblockparse)
		return (*this);
	this->_http_keyword_check = httpblockparse._http_keyword_check;
	this->_http_braket_open = httpblockparse._http_braket_open;
	this->_http_block_count = httpblockparse._http_block_count;
	this->_root_flag = httpblockparse._root_flag;
	this->_autoindex_flag = httpblockparse._autoindex_flag;
	this->_default_err_page_flag = httpblockparse._default_err_page_flag;
	this->_root = httpblockparse._root;
	this->_autoindex = httpblockparse._autoindex;
	this->_default_err_page = httpblockparse._default_err_page;
	this->_err_page_directory = httpblockparse._err_page_directory;
	this->_http_parse_done = httpblockparse._http_parse_done;
	this->_config_file_name = httpblockparse._config_file_name;
	this->_err_msg = httpblockparse._err_msg;
	this->_http_block = httpblockparse._http_block;
	return (*this);
}

HttpBlockParse::~HttpBlockParse()
{
}

std::string	HttpBlockParse::GetRoot(void) const
{
	return (this->_root);
}

std::string	HttpBlockParse::GetAutoIndex(void) const
{
	return (this->_autoindex);
}

std::string	HttpBlockParse::GetDefaultErrorPage(void) const
{
	return (this->_default_err_page);
}

std::string	HttpBlockParse::GetErrPageDirectory(void) const
{
	return (this->_err_page_directory);
}

int	HttpBlockParse::GetHttpParseDone(void) const
{
	return (this->_http_parse_done);
}

t_http_block	HttpBlockParse::GetHttpBlock(void) const
{
	return (this->_http_block);
}

void	HttpBlockParse::SetConfigFileName(std::string config_file_name)
{
	this->_config_file_name = config_file_name;
}

void	HttpBlockParse::SaveHttpBlock(std::vector<t_server_block> server_block)
{
	this->_http_block.root = this->_root;
	this->_http_block.autoindex = this->_autoindex;
	this->_http_block.default_error_page = this->_default_err_page;
	this->_http_block.err_page_directory = this->_err_page_directory;
	this->_http_block.server_block = server_block;
}

void	HttpBlockParse::HttpBlockCheck(std::string *line, int i)
{
	std::string	temp = *line;
	
	/* when the line includes keyword "server", regard as http block parsing is done */
	if (temp.find("server") != std::string::npos)
		this->_http_parse_done++;
	/* if http block parsing is done, it will not be handled on this function */
	if (this->_http_parse_done)
		return ;
	
	/* handled every closing curved braket in the location block parsing function */
	if (temp.find("}") != std::string::npos)
	{
		this->_err_msg = ErrMsg(this->_config_file_name, HTTP_BRAKET_CLOSE, *line, i);
		throw (this->_err_msg);
	}
	/* opening curved braket exists only once in the http block */
	if (this->_http_braket_open && temp.find("{") != std::string::npos)
	{
		this->_err_msg = ErrMsg(this->_config_file_name, HTTP_BRAKET_OPEN, *line, i);
		throw (this->_err_msg);
	}

	if (temp.find("http") != std::string::npos)
		HttpKeywordHttpCheck(line, temp, i);
	else if (temp.find("{") != std::string::npos)
		HttpBraketOpenCheck(line, temp, i);
	else if (temp.find("root") != std::string::npos
		|| temp.find("autoindex") != std::string::npos
		|| temp.find("default_error_page") != std::string::npos)
	{
		if (temp.find("root") != std::string::npos)
			HttpKeywordCheck(line, temp, i, "root");
		if (temp.find("autoindex") != std::string::npos)
			HttpKeywordCheck(line, temp, i, "autoindex");
		if (temp.find("default_error_page") != std::string::npos)
			HttpKeywordCheck(line, temp, i, "default_error_page");
		HttpKeywordTokenCheck(line, temp, i);
	}
	else
	{
		if (StringCheck(temp))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_INVALID_KWD, *line, i);
			throw (this->_err_msg);
		}
	}
}

void	HttpBlockParse::HttpKeywordCheck(std::string *line, std::string temp, int i, std::string keyword)
{
	if (StringCheck(temp.substr(0, temp.find(keyword)))
		|| !(temp[temp.find(keyword) + strlen(keyword.c_str())] == ' ' || temp[temp.find(keyword) + strlen(keyword.c_str())] == '\t'))
	{
		if (keyword == "http")
		{
			if (temp[temp.find(keyword) + strlen(keyword.c_str())] == '{'
				|| temp[temp.find(keyword) + strlen(keyword.c_str())] == '\0')
				return ;
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_HTTP, *line, i);
		}
		else if (keyword == "root")
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_ROOT, *line, i);
		else if (keyword == "autoindex")
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_AUTOINDEX, *line, i);
		else
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_DEFAULT_ERROR_PAGE, *line, i);
		throw (this->_err_msg);
	}
}

void	HttpBlockParse::HttpKeywordHttpCheck(std::string *line, std::string temp, int i)
{
	/* keyword "http" exists only once in the http block */
	HttpKeywordCheck(line, temp, i, "http");
	this->_http_keyword_check++;

	/* opening curved braket should be exists after the keyword "http" */
	if (this->_http_keyword_check == 0 && StringCheck(temp.substr(temp.find("http") + strlen("http"), std::string::npos), '{'))
	{
		this->_err_msg = ErrMsg(this->_config_file_name, HTTP_BRAKET_OPEN, *line, i);
		throw (this->_err_msg);
	}

	if (temp.find("{") != std::string::npos)
	{
		temp = temp.substr(temp.find("http") + strlen("http"), std::string::npos);
		HttpBraketOpenCheck(line, temp, i);
	}
}

void	HttpBlockParse::HttpBraketOpenCheck(std::string *line, std::string temp, int i)
{
	this->_http_braket_open++;
	if (temp.find("{") != 0 && !StringCheck(temp.substr(0, temp.find("{"))))
		temp = temp.substr(temp.find("{"), std::string::npos);

	/* opening curved braket exists only once in the http block */
	if (temp.substr(temp.find("{") + strlen("{"), std::string::npos).find("{") != std::string::npos)
	{
		this->_err_msg = ErrMsg(this->_config_file_name, HTTP_BRAKET_OPEN, *line, i);
		throw (this->_err_msg);

	}
	if (temp.find("root") != std::string::npos
		|| temp.find("autoindex") != std::string::npos
		|| temp.find("default_error_page") != std::string::npos)
	{
		temp = temp.substr(temp.find("{") + 1, std::string::npos);
		if (temp.find("root") != std::string::npos)
			HttpKeywordCheck(line, temp, i, "root");
		if (temp.find("autoindex") != std::string::npos)
			HttpKeywordCheck(line, temp, i, "autoindex");
		if (temp.find("default_error_page") != std::string::npos)
			HttpKeywordCheck(line, temp, i, "default_error_page");
		HttpKeywordTokenCheck(line, temp, i);
	}

	/* after finding opening braket, there should be keyword "http" too */
	if (this->_http_keyword_check == 1 && this->_http_braket_open == 1)
		this->_http_block_count++;
	else
	{
		this->_err_msg = ErrMsg(this->_config_file_name, HTTP_BRAKET_OPEN, *line, i);
		throw (this->_err_msg);
	}
}

void	HttpBlockParse::HttpKeywordTokenCheck(std::string *line, std::string temp, int i)
{
	std::string	token[100] = {"", };

	if (temp.find("root") != std::string::npos)
	{
		if (!this->_http_braket_open)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_BRAKET_OPEN, *line, i);
			throw (this->_err_msg);
		}
		if (this->_root_flag == 1)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_ROOT_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (TokenCount(2, temp, token))
		{
			if (!(TokenCount(temp, token) == 3 && token[1].find(";") == std::string::npos))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_ROOT, *line, i);
				throw (this->_err_msg);
			}
		}
	}
	else if (temp.find("autoindex") != std::string::npos)
	{
		if (!this->_http_braket_open)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_BRAKET_OPEN, *line, i);
			throw (this->_err_msg);
		}
		if (this->_autoindex_flag == 1)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_AUTOINDEX_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (TokenCount(2, temp, token) && !this->_http_parse_done)
		{
			if (!(TokenCount(temp, token) == 3 && token[1].find(";") == std::string::npos))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_AUTOINDEX, *line, i);
				throw (this->_err_msg);
			}
		}
	}
	else if (temp.find("default_error_page") != std::string::npos)
	{
		if (!this->_http_braket_open)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_BRAKET_OPEN, *line, i);
			throw (this->_err_msg);
		}
		if (this->_default_err_page_flag == 1)
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_DEFAULT_ERROR_PAGE_EXISTS, *line, i);
			throw (this->_err_msg);
		}
		if (TokenCount(2, temp, token) && !this->_http_parse_done)
		{
			if (!(TokenCount(temp, token) == 3 && token[1].find(";") == std::string::npos))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_DEFAULT_ERROR_PAGE, *line, i);
				throw (this->_err_msg);
			}
		}
	}
	HttpBlockGetInfo(token, line, i);
	if (this->_root != "" && this->_autoindex != "" && this->_default_err_page != "")
		this->_http_parse_done = 1;
}

void	HttpBlockParse::HttpBlockGetInfo(std::string *token, std::string *line, int i)
{
	if (token[0] == "root" && !this->_http_parse_done)
	{
		if (SemicolonCheck(token[1]))
		{
			if (!(token[2] != "" && token[1].find(";") == std::string::npos && !SemicolonCheck(token[2])))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, HTTP_SEMICOLON, *line, i);
				throw (this->_err_msg);
			}
		}
		if (token[2] == "")
		{
			token[1] = token[1].substr(0, token[1].find(";"));
			token[1] = RemoveSpaceTab(token[1]);
		}
		if (!CheckDirectory(token[1], ROOT))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_ROOT_VALUE, *line, i);
			throw (this->_err_msg);
		}
		this->_root_flag++;
		this->_root = token[1];
	}
	if (token[0] == "autoindex" && !this->_http_parse_done)
	{
		if (SemicolonCheck(token[1]))
		{
			if (!(token[2] != "" && token[1].find(";") == std::string::npos && !SemicolonCheck(token[2])))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, HTTP_SEMICOLON, *line, i);
				throw (this->_err_msg);
			}
		}
		if (token[2] == "")
		{
			token[1] = token[1].substr(0, token[1].find(";"));
			token[1] = RemoveSpaceTab(token[1]);
		}
		if (!(token[1] == "on" || token[1] == "off"))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_AUTOINDEX, *line, i);
			throw (this->_err_msg);
		}
		this->_autoindex_flag++;
		this->_autoindex = token[1];
	}
	if (token[0] == "default_error_page" && !this->_http_parse_done)
	{
		if (SemicolonCheck(token[1]))
		{
			if (!(token[2] != "" && token[1].find(";") == std::string::npos && !SemicolonCheck(token[2])))
			{
				this->_err_msg = ErrMsg(this->_config_file_name, HTTP_SEMICOLON, *line, i);
				throw (this->_err_msg);
			}
		}
		if (token[2] == "")
		{
			token[1] = token[1].substr(0, token[1].find(";"));
			token[1] = RemoveSpaceTab(token[1]);
		}
		this->_default_err_page_flag++;
		this->_default_err_page = this->_root + token[1];

		std::stringstream	ss(this->_default_err_page);
		std::string		word[100];
		int			i = 0;
		while (!ss.eof())
		{
			getline(ss, word[i], '/');
			i++;
		}
		std::string	err_page_directory = "";
		for (int j = 0; j < i - 1; j++)
		{
			err_page_directory.append(word[j]);
			if (j != i - 2)
				err_page_directory += '/';
		}
		this->_err_page_directory = err_page_directory;
		if (!(CheckDirectory(this->_err_page_directory, 0)
			&& this->_err_page_directory.find("error_pages") != std::string::npos
			&& (this->_default_err_page.find("error_pages/400.html") != std::string::npos
				|| this->_default_err_page.find("error_pages/403.html") != std::string::npos
				|| this->_default_err_page.find("error_pages/404.html") != std::string::npos
				|| this->_default_err_page.find("error_pages/405.html") != std::string::npos
				|| this->_default_err_page.find("error_pages/413.html") != std::string::npos
				|| this->_default_err_page.find("error_pages/500.html") != std::string::npos
				|| this->_default_err_page.find("error_pages/505.html") != std::string::npos)))
		{
			this->_err_msg = ErrMsg(this->_config_file_name, HTTP_KWD_DEFAULT_ERROR_PAGE_VALUE, *line, i);
			throw (this->_err_msg);
		}
	}
}
