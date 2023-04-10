/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 13:54:27 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/05 14:02:04 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Parse.hpp"

Parse::Parse()
{
}

Parse::Parse(std::string config_file_name)
{
	FileOpen(config_file_name);
}

Parse::Parse(const Parse& parse): LocationBlockParse(parse)
{
	*this = parse;
}


Parse& Parse::operator=(const Parse& parse)
{
	if (this == &parse)
		return (*this);
	
	this->_http_block = parse._http_block;
	return (*this);
}

Parse::~Parse()
{
}

t_http_block	Parse::GetHttpBlock(void) const
{
	return (this->_http_block);	
}

int	Parse::FileOpen(std::string config_file_name)
{
	std::fstream	f_read;

	f_read.open(config_file_name.c_str(), std::ios::in);
	if (f_read.fail())
	{
		std::string	err_msg = ErrMsg("", FILE_OPEN_ERROR, "", -1);
		throw (err_msg);
		return (1);
	}

	HttpBlockParse::SetConfigFileName(config_file_name);
	ServerBlockParse::SetConfigFileName(config_file_name);
	LocationBlockParse::SetConfigFileName(config_file_name);

	std::string	line;
	int		i = 0;
	while (std::getline(f_read, line))
	{
		i++;
		if (line[0] == '#' || line == "")
			continue ;
		if (line.find("#"))
			line = line.substr(0, line.find("#"));

		if (!HttpBlockParse::GetHttpParseDone())
			HttpBlockParse::HttpBlockCheck(&line, i);
		else if (!ServerBlockParse::GetServerParseDone())
			ServerBlockParse::ServerBlockCheck(&line, i);
		else if (!LocationBlockParse::GetLocationParseDone())
			LocationBlockParse::LocationBlockCheck(&line, i);
			
		//std::cout << line << std::endl;
	}

	if (!HttpBlockParse::GetHttpParseDone())
	{
		std::string	err_msg;
		err_msg = ErrMsg(config_file_name, HTTP_KWD_HTTP_MISSED, "", -1);
		throw (err_msg);
	}

	this->_http_block =  HttpBlockParse::GetHttpBlock();
	PrintHttpBlockInfo();

	f_read.close();
	return (0);
}

void	Parse::PrintHttpBlockInfo(void)
{
	std::cout << _CYAN << "[HTTP BLOCK]" << _BLACK << std::endl;
	std::cout << "root\t\t\t\t: " << _CYAN << this->_http_block.root << _BLACK << std::endl;
	std::cout << "autoindex\t\t\t: " << _CYAN << this->_http_block.autoindex << _BLACK << std::endl;
	std::cout << "default_error_page\t\t: " << _CYAN << this->_http_block.default_error_page << _BLACK << std::endl;
	std::cout << "error_page_directory\t\t: " << _CYAN << this->_http_block.err_page_directory << _BLACK << std::endl;
	std::cout << std::endl;
	std::cout << _CYAN << "total server block\t\t\t: " << this->_http_block.server_block.size() << _BLACK << std::endl;
	std::cout << std::endl;

	int	index = 1;
	for (std::vector<t_server_block>::iterator iter = this->_http_block.server_block.begin();
		iter != this->_http_block.server_block.end(); iter++)
	{
		std::cout << _YELLOW << "[SERVER BLOCK_" << index << "]" << _BLACK << std::endl;
		std::cout << "listen\t\t\t\t: " << _YELLOW << (*iter).listen << _BLACK << std::endl;
		std::cout << "host\t\t\t\t: " << _YELLOW << (*iter).host << _BLACK << std::endl;
		std::cout << "client_max_body_size\t\t: " << _YELLOW << (*iter).client_max_body_size << _BLACK << std::endl;
		std::cout << "index\t\t\t\t: " << _YELLOW << (*iter).index << _BLACK << std::endl;
		std::cout << "allow_methods\t\t\t: ";
		if ((*iter).allow_methods_get == 0 && (*iter).allow_methods_post == 0 && (*iter).allow_methods_delete == 0)
			std::cout << _YELLOW << "UNKNOWN" << _BLACK << std::endl;
		else if ((*iter).allow_methods_get == 1 && (*iter).allow_methods_post == 0 && (*iter).allow_methods_delete == 0)
			std::cout << _YELLOW << "GET" << _BLACK << std::endl;
		else if ((*iter).allow_methods_get == 0 && (*iter).allow_methods_post == 1 && (*iter).allow_methods_delete == 0)
			std::cout << _YELLOW << "POST" << _BLACK << std::endl;
		else if ((*iter).allow_methods_get == 0 && (*iter).allow_methods_post == 0 && (*iter).allow_methods_delete == 1)
			std::cout << _YELLOW << "DELETE" << _BLACK << std::endl;
		else if ((*iter).allow_methods_get == 1 && (*iter).allow_methods_post == 1 && (*iter).allow_methods_delete == 0)
			std::cout << _YELLOW << "GET POST" << _BLACK << std::endl;
		else if ((*iter).allow_methods_get == 1 && (*iter).allow_methods_post == 0 && (*iter).allow_methods_delete == 1)
			std::cout << _YELLOW << "GET DELETE" << _BLACK << std::endl;
		else if ((*iter).allow_methods_get == 0 && (*iter).allow_methods_post == 1 && (*iter).allow_methods_delete == 1)
			std::cout << _YELLOW << "POST DELETE" << _BLACK << std::endl;
		else
			std::cout << _YELLOW << "GET POST DELETE" << _BLACK << std::endl;
		std::cout << "save_path\t\t\t: " << _YELLOW << (*iter).save_path << _BLACK << std::endl;
		std::cout << std::endl;
		std::cout << _YELLOW << "total redirection location block\t: " << (*iter).redirection_location_block.size() << _BLACK << std::endl;
		std::cout << _YELLOW << "total index location block\t\t: " << (*iter).index_location_block.size() << _BLACK << std::endl;
		std::cout << _YELLOW << "total cgi-bin location block\t\t: " << (*iter).cgi_location_block.size() << _BLACK << std::endl;
		std::cout << std::endl;

		int i = 1;
		if ((*iter).redirection_location_block.size() != 0)
		{
			std::cout << _MAGENTA << "[REDIRECTION LOCATION BLOCK]" << _BLACK << std::endl;
			for (std::vector<t_redirection>::iterator it = (*iter).redirection_location_block.begin();
				it != (*iter).redirection_location_block.end(); it++)
			{
				std::cout << _MAGENTA << "[" << i << "]" << _BLACK << std::endl;
				std::cout << "location path\t\t\t: " << _MAGENTA << (*it).path << _BLACK << std::endl;
				std::cout << "return\t\t\t\t: " << _MAGENTA << (*it).return_value << _BLACK << std::endl;
				std::cout << std::endl;
				i++;
			}
		}
		
		i = 1;
		if ((*iter).index_location_block.size() != 0)
		{
			std::cout << _MAGENTA << "[INDEX LOCATION BLOCK]" << _BLACK << std::endl;
			for (std::vector<t_index>::iterator it = (*iter).index_location_block.begin();
				it != (*iter).index_location_block.end(); it++)
			{
				std::cout << _MAGENTA << "[" << i << "]" << _BLACK << std::endl;
				std::cout << "location path\t\t\t: " << _MAGENTA << (*it).path << _BLACK << std::endl;
				std::cout << "index\t\t\t\t: " << _MAGENTA << (*it).index << _BLACK << std::endl;
				std::cout << std::endl;
				i++;
			}
		}

		std::cout << _MAGENTA << "[CGI LOCATION BLOCK]" << _BLACK << std::endl;
		if ((*iter).cgi_location_block.size() == 1)
		{
			std::vector<t_cgi>::iterator it = (*iter).cgi_location_block.begin();
			
			std::cout << "location path\t\t\t: " << _MAGENTA << (*it).path << _BLACK << std::endl;
			std::cout << "cgi_path\t\t\t: " << _MAGENTA << (*it).cgi_path << _BLACK << std::endl;
			if ((*it).cgi_extention == 1)
				std::cout << "cgi_extention\t\t\t: " << _MAGENTA << ".php" << _BLACK << std::endl;
			std::cout << std::endl;
		}
		index++;
	}
}
