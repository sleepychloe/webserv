/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHeaders.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 23:24:00 by hyunah            #+#    #+#             */
/*   Updated: 2023/03/04 23:24:03 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/MessageHeaders.hpp"
#include <cstdio>
MessageHeaders::MessageHeaders(){}

MessageHeaders::~MessageHeaders(){}

MessageHeaders::MessageHeaders(MessageHeaders const &src){
	*this = src;
}
MessageHeaders &MessageHeaders::operator=(MessageHeaders const &rhs){
	if (this != &rhs)
	{
		this->headers = rhs.headers;
		this->body = rhs.body;
	}
	return (*this);
}

std::string	stripMarginWhitespace(const std::string &s)
{
	const	char	*whitespace = " \t\r\n";
	std::size_t		marginLeft;
	std::size_t		marginRight;

	marginLeft = s.find_first_not_of(whitespace);
	marginRight = s.find_last_not_of(whitespace);
	if (marginLeft == std::string::npos)
		return "";
	else
		return s.substr(marginLeft, marginRight - marginLeft + 1);
}

bool	MessageHeaders::parseFromString(const std::string &rawMsg, size_t & bodyOffset){
	std::size_t	offset = 0;
	std::size_t lineTerminator;
	std::size_t nameTerminator;
	std::string	rest;
	std::string	header;
	std::string	endl = "\r\n";

	while(this->headers.size() != 0)
		headers.pop_back();
	lineTerminator = rawMsg.find(endl);
	if (lineTerminator != std::string::npos)
		rest = rawMsg;
	while (lineTerminator != std::string::npos)
	{
		if (lineTerminator == 0)
		{
			offset+= endl.length();
			break;
		}
		header = rest.substr(0, lineTerminator);
		// std::cout << "\t parsing : "<<header << std::endl;
		offset+= lineTerminator + endl.length();
		nameTerminator = header.find(":");
		if (nameTerminator == std::string::npos)
			return false;
		Header tmp;
		tmp.name = stripMarginWhitespace(header.substr(0, nameTerminator));
		// std::cout << "\t parsing name: "<<tmp.name << std::endl;
		tmp.value = stripMarginWhitespace(header.substr(nameTerminator + 1));
		// std::cout << "\t parsing value: "<<tmp.value << std::endl;
		headers.push_back(tmp);
		rest = rest.substr(lineTerminator + endl.length());
		lineTerminator = rest.find(endl);
	}
	bodyOffset = offset;
	// std::cout << "rest :" << rest << std::endl;
	nameTerminator = rest.find(":");
	if (nameTerminator != std::string::npos)
	{
		Header tmp;
		tmp.name = stripMarginWhitespace(rest.substr(0, nameTerminator));
		std::cout << "\t parsing name: "<<tmp.name << std::endl;
		tmp.value = stripMarginWhitespace(rest.substr(nameTerminator + 1));
		std::cout << "\t parsing value: "<<tmp.value << std::endl;
		headers.push_back(tmp);
		return (true);
	}
	if (rest.find(endl) == std::string::npos)
		return (printf("No body delim\n"), false);
	// printf("6, endl.length() %li\n", endl.length());
	// body = rest.substr(endl.length());
	// printf("7\n");
	return (true);
}

bool	MessageHeaders::parseFromString(const std::string &rawMsg){
	size_t	bodyOffset;

	return (this->parseFromString(rawMsg, bodyOffset));
}

MessageHeaders::Headers	MessageHeaders::getHeaders() const{
	return (headers);
}

bool	MessageHeaders::hasHeader(const std::string& name) const{
	for (unsigned i=0; i != headers.size(); i++)
	{
		if (headers[i].name == name)
			return (true);
	}
	return (false);
}

std::string	MessageHeaders::getBody() const{
	return (body);
}

std::string	MessageHeaders::generateRawMsg(){
	std::ostringstream buffer;

	for (unsigned i=0; i != headers.size(); i++)
	{
		buffer << headers[i].name << ": " << headers[i].value << "\r\n";
	}
	buffer << "\r\n";
	// buffer << body;
	return (buffer.str());
}

std::string	MessageHeaders::getHeaderValue(std::string name) const {
	for (unsigned i=0; i != headers.size(); i++)
	{
		if (headers[i].name == name)
			return (headers[i].value);
	}
	return ("");
}

void	MessageHeaders::setBody(const std::string newBody){
	body = newBody;
}

void	MessageHeaders::addHeader(std::string name, std::string value){
	Header tmp;
	tmp.name = stripMarginWhitespace(name);
	tmp.value = stripMarginWhitespace(value);
	headers.push_back(tmp);
}

void	MessageHeaders::setHeaderValue(std::string name, std::string value)
{
	for (unsigned i=0; i != headers.size(); i++)
	{
		if (headers[i].name == name)
		{
			headers[i].value = value;
			return ;
		}
	}
	return ;
}


std::ostream & operator<<(std::ostream & o, MessageHeaders & rhs){
    o << " < PRINT MessageHeaders > " << std::endl;
	for (long unsigned int i = 0; i < rhs.getHeaders().size(); i++)
	{
		o << "\t"<< i << ". " << rhs.getHeaders().at(i).name << " : " << rhs.getHeaders().at(i).value << std::endl;
	}
    o << "\n________________" << std::endl;
	return (o);
}
