/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:53:25 by hyunah            #+#    #+#             */
/*   Updated: 2023/03/30 15:54:47 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_H
# define CGI_H
# include "Request.hpp"
# include "Server.hpp"
# include <map>
# include <vector>
# include <fstream>

// class Server;

class Cgi
{
	struct m_File{
		std::string			tmpLoc;
		std::string			name;
		std::string			var;
		std::string			type;
		std::vector<char>	data;
		MessageHeaders		headers;
	};

private:
	std::string							scriptPath;
	std::string							cmd;
	std::map<std::string, std::string>	cmdMap;
	std::vector<std::string>			env;

public:
	Cgi();
	Cgi(Cgi & src);
	Cgi &operator=(Cgi & rhs);
	~Cgi();

	int				analyse(Server *server, Request *request);
	std::string 	getScriptPath() const;
	std::string		getCmd() const;
	void			addEnvParam(std::string key, std::string value);
	void			addEnvParam(std::string str);
	char			**getPathArray();
	char			**getEnvArray();
	m_File			file;
	bool			parsingFileBody(std::vector<char> data, MessageHeaders headers, int maxClientBodySize, unsigned int *errorCode);
	bool			upload();
	std::vector<char>	execute();
};

#endif