/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 23:24:00 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/07 16:16:58 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client()
{
}

Client::~Client()
{
}

// bool parseStatusLine(Response *response, std::string responseLine){
// 	//parse the protocal
// 	std::size_t	protocalDelimiter = responseLine.find(' ');
// 	if (protocalDelimiter == std::string::npos)
// 		return (false);
// 	if (responseLine.substr(0, protocalDelimiter) != "HTTP/1.1")
// 		return (false);
		
// 	//parse the statusCode
// 	std::size_t	statusCodeDelimiter = responseLine.find(' ', protocalDelimiter + 1);
// 	unsigned int statusCode = atoi(responseLine.substr(protocalDelimiter + 1, statusCodeDelimiter - protocalDelimiter - 1).c_str());
// 	if (statusCode > 999)
// 		return (false);
// 	else
// 		response->statusCode = statusCode;

// 	//parse the reasonPhrase
// 	response->reasonPhrase = responseLine.substr(statusCodeDelimiter + 1);
// 	return (true);
// }

// Response*	Client::parseResponse(const std::string &rawReponse){
// 	std::string	CRLF = "\r\n";
// 	std::size_t	statusLineEnd = rawReponse.find(CRLF);
	
// 	if (statusLineEnd == std::string::npos)
// 		return (NULL);
// 	std::string	responseLine = rawReponse.substr(0, statusLineEnd);
// 	if (!parseStatusLine(&response, responseLine))
// 		return (NULL);
// 	if (!response.headers.parseFromString(rawReponse.substr(statusLineEnd + CRLF.length())))
// 		return (NULL);
// 	if (response.headers.hasHeader("Content-Length"))
// 	{
// 		std::size_t	contentLength = atoi(response.headers.getHeaderValue("Content-Length").c_str());
// 		if (contentLength > response.headers.getBody().length())
// 			return (NULL);
// 		else
// 		{
// 			std::string newBody = response.headers.getBody().substr(0, contentLength);
// 			response.headers.setBody(newBody);
// 		}
// 	}
// 	return (&response);
// }
