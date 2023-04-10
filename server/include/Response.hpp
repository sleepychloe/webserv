/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 12:13:27 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/07 21:26:07 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_H
# define RESPONSE_H
# include "Uri.hpp"
# include "MessageHeaders.hpp"
# include "Server.hpp"
# include "Cgi.hpp"
# include "Request.hpp"
# include "utils.hpp"
# include <map>
# include <ctime>
# include <fstream>
# include <iostream>

class Response
{
private:
	std::map<int, std::string>			statusCodeDic;
	std::map<std::string, std::string>	mimeMap;
	std::string							getMimeType(std::string & filepath);
	std::string							getFormat(std::string & filepath);
public:
	Response();
	~Response();
	int					cgiState;
	unsigned int		statusCode;
	std::string			reasonPhrase;
	Uri					target;
	MessageHeaders		headers;
	std::vector<char>	fileToBinary(std::string filename);
	std::string			status;
	std::vector<char>	data;
	std::string			generateDateHeader();
	std::string			generateRawResponse(int code, MessageHeaders msg, std::string body);
	std::vector<char>	getMethod(Server &server, Request *request, std::size_t messageEnd);
	std::vector<char>	postMethod(Server &server, Request *request, std::size_t messageEnd);
	std::vector<char>	deleteMethod(Server &server, Request *request, std::size_t messageEnd);
	std::vector<char>	buildResponse(Server & server, Request * request, std::string dir, int errorCode);
	std::vector<char>	buildErrorResponse(std::string dir, int errorCode);
	std::vector<char>	buildResponseForCgi(std::vector<char> data, int code);
	std::vector<char>	buildResponseforAutoIndex(Request * request, std::string dir, int code);
	std::vector<char>	buildResponseForRedirection(Server &server, Request * request, std::string dir, int code);
	std::string			getFileDateTime(time_t sec);
};

#endif
