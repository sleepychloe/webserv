/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:35:23 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/07 19:07:57 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"

Request::Request(){}

Request::~Request(){}

Request::Request(Request const &src){
	*this = src;
}

Request &Request::operator=(Request const &rhs){
	if (this != &rhs)
	{
		this->body = rhs.body;
		this->protocol = rhs.protocol;
		this->method = rhs.method;
		this->target = rhs.target;
		this->headers = rhs.headers;
	}
	return (*this);
}


bool	parseRequestLine(Request *request, const std::string requestLine)
{
	//parse the method
	std::size_t	methodDelimiter = requestLine.find(' ');
	if (methodDelimiter == std::string::npos)
		return (false);
	request->method = requestLine.substr(0, methodDelimiter);

	//parse the target URI
	std::size_t	targetDelimiter = requestLine.find(' ', methodDelimiter + 1);
	request->target.parsingFromString(requestLine.substr(methodDelimiter + 1, targetDelimiter - methodDelimiter - 1));
	// //parse the protocol
	request->protocol = requestLine.substr(targetDelimiter + 1);
	return (true);
}

bool	parseSize(const	std::string & numberString, size_t	& number)
{
	const	char *str = numberString.c_str();
	number = 0;
	for (unsigned char i= 0; i < numberString.length(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		// detect overflow
		size_t previousNumber = number;
		number *= 10;
		number += (unsigned short)(str[i] - '0');
		if (number / 10 != previousNumber)
			return false;
	}
	return true;
}
#include <cstdio>

bool	Request::parseResquest(std::vector<char> rawRequest, size_t & messageEnd){
	std::string	CRLF = "\r\n";

	// parse the request line.
	std::size_t	requestLineEnd	= vecFind(rawRequest, CRLF);	
	if (requestLineEnd == std::string::npos)
		return (printf("No request Line End\n"), false);
	std::string	requestLine = vecSubstr(rawRequest, 0, requestLineEnd);
	if (!parseRequestLine(this, requestLine))
		return (printf("request Line Not Parsable\n"), false);
	// printf("request Line PARSING OK\n");

	// parse the headers line.
	size_t	bodyOffset = 0;
	size_t	headerOffset = requestLineEnd + CRLF.length();
	std::string	header = vecSubstr(rawRequest, headerOffset, rawRequest.size());
	if (!this->headers.parseFromString(header, bodyOffset))
		return (printf("Header Line End\n"), false);
	// printf("Header Line PARSING OK\n");
	// check for content-length header. if present, use this to determine how many character should be in the body.
	bodyOffset += headerOffset;
	size_t	maxContentLength = rawRequest.size() - bodyOffset;
	size_t	contentLength;
	// extract body.
	if (this->headers.hasHeader("Content-Length"))
	{
		if (!parseSize(this->headers.getHeaderValue("Content-Length"), contentLength))
			return (false);
			// return (printf("Content-length Size parsing\n"), false);
		if (contentLength > maxContentLength)
		{
			return (false);
			// return (printf("Content-length > MaxContentLength\n"), false);
		}
		else
			messageEnd = bodyOffset + contentLength + CRLF.length();
	}
	else
		messageEnd = bodyOffset;
	std::string	doubleCRLF = "\r\n\r\n";
	messageEnd = vecFind(rawRequest, doubleCRLF) + 4;
	printf("messageEnd: %li\n", messageEnd);
	if (messageEnd == std::string::npos)
		return (false);
	printf("Body?:\n");
	std::vector<char> filebody;

	filebody.insert(filebody.begin(), rawRequest.begin() + messageEnd, rawRequest.end());
	// printData(filebody);
	this->body = filebody;
	// std::cout << "PARSABLE\n";
	
	return (true);
}
	
bool	Request::parseResquest(const std::vector<char> rawRequest){
	std::size_t	messageEnd;

	if (!this->parseResquest(rawRequest, messageEnd))
		return (false);
	return (true);
}

std::ostream & operator<<(std::ostream & o, Request & rhs){
    o << "-------PRINT Request--------" << std::endl;
	o << "method : " << rhs.method << std::endl;
	o << "protocol : " << rhs.protocol << std::endl;
	o << "target : " << rhs.target.generateString() << std::endl;
	o << "headers : " << rhs.headers << std::endl;
	o << "bodysize : " << rhs.body.size() << std::endl;
	o << "body : ";
	for (std::vector<char>::iterator it = rhs.body.begin(); it != rhs.body.end(); ++it)
	{
		o << *it;
	}
    o << std::endl<<"-------------------------------" << std::endl;
    return (o);
}

