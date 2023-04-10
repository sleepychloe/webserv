/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uri.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 11:25:10 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/02 20:30:12 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Uri.hpp"
#include <iostream>
#include <sstream>

Uri::Uri() : existPath(false), existPort(true), port(0), scheme (""), host(""), splitchar("/"){}

Uri::~Uri(){}

Uri::Uri(Uri const & src){
	*this = src;
}

Uri &Uri::operator=(Uri const & rhs){
	if (this != &rhs)
	{
		this->existPath = rhs.existPath;
		this->existPort = rhs.existPort;
		this->port = rhs.port;
		this->scheme = rhs.scheme;
		this->host = rhs.host;
		this->splitchar = rhs.splitchar;
		this->fragment = rhs.fragment;
		this->query = rhs.query;
		this->path = rhs.path;
	}
	return (*this);
}

std::string		Uri::parseScheme(std::string uriString)
{
	std::size_t schemeDelimiter = uriString.find(':');
	std::string	rest;
	if (schemeDelimiter == std::string::npos)
	{
		rest = uriString;
	}
	else
	{
		rest = uriString.substr(schemeDelimiter + 1);
		scheme = uriString.substr(0, schemeDelimiter);
	}
	return (rest);
}

void	Uri::parsePath(std::string & pathString)
{
	while (!pathString.empty() && existPath)
	{
		std::size_t	pathschemeDelimiter = pathString.find(splitchar);
		if (pathschemeDelimiter == 0 && pathString.length()== 1)
		{
			path.push_back("");
			break ;
		}			
		else if (pathschemeDelimiter == std::string::npos)
		{
			path.push_back(pathString);
			break ;
		}
		else
		{
			std::string tmp = pathString.substr(0, pathschemeDelimiter);
			path.push_back(tmp);
		}
		// if (pathschemeDelimiter + 1 == pathString.length())
		// 	path.push_back("");
		existPath = true;
		pathString = pathString.substr(pathschemeDelimiter + splitchar.length());
	}
}

void	Uri::parseFragment(std::string & queryAndOrFrag)
{
	std::size_t	fragschemeDelimiter = queryAndOrFrag.find('#');
	std::string	leftover;
	if (fragschemeDelimiter == std::string::npos)
	{
		fragment = "";
		leftover = queryAndOrFrag;
	}
	else
	{
		leftover = queryAndOrFrag.substr(0, fragschemeDelimiter);
		fragment = queryAndOrFrag.substr(fragschemeDelimiter + 1);
	}
	if (!leftover.empty())
		query = leftover.substr(1);
	
}
bool	Uri::parseValidPort(std::string &pathString, int portDelimeter, int authorityEnd)
{
	long long port_tmp = 0;

	if (pathString.substr(portDelimeter + 1, authorityEnd - portDelimeter - 1).find_first_not_of("0123456789") != std::string::npos)
		return (false);
	port_tmp = (long long)atoi(pathString.substr(portDelimeter + 1, authorityEnd - portDelimeter - 1).c_str());
	if (port_tmp == 0 && pathString.substr(portDelimeter + 1, 1) != "0")
		return (false);
	if (port_tmp < 0 || port_tmp > 65535)
		return (false);
	port = (unsigned short)port_tmp;
	existPort = true;
	return (true);
}

std::string	Uri::findQueryFragment(std::string & rest,std::string & queryAndOrFrag){
	std::string	pathString;
	std::size_t	pathEnd = rest.find_first_of("?#");
	
	if (pathEnd == std::string::npos)
		pathString = rest;
	else
	{
		pathString = rest.substr(0, pathEnd);
		queryAndOrFrag = rest.substr(pathString.length());
	}
	return (pathString);
}

bool	Uri::parsingFromString(const std::string & uriString){
	scheme = "";
	host = "";
	query = "";
	fragment = "";
	path.clear();
	port = 0;
	existPort = false;
	existPath = true;
	std::string	rest;
	std::string	queryAndOrFrag;

	rest = parseScheme(uriString);
	rest = findQueryFragment(rest, queryAndOrFrag);
	if (rest.substr(0, 2) == "//")
	{
		std::size_t	authorityEnd = rest.find(splitchar, 2);
		if (authorityEnd == std::string::npos)
		{
			existPath = false;
			authorityEnd = rest.length();
		}
		std::size_t	portDelimeter = rest.find(":", 2);
		if (portDelimeter == std::string::npos)
		{
			if (existPath)
				host = rest.substr(2, authorityEnd - 2);
			else
				host = rest.substr(2);
		}
		else
		{
			host = rest.substr(2, portDelimeter - 2);
			if (!parseValidPort(rest, portDelimeter, authorityEnd))
				return (false);
		}
		rest = rest.substr(authorityEnd);
	}
	parsePath(rest);
	parseFragment(queryAndOrFrag);
	return (true);
}

std::string	Uri::getScheme() const{
	return (scheme);
}

std::string	Uri::getFragement() const{
	return (fragment);
}

std::string	Uri::getQuery() const{
	return (query);
}

std::string	Uri::getHost() const{
	return (host);
}

std::vector<std::string> Uri::getPath(){
	return (path);
}

void	Uri::setSplitChar(const std::string & newchar){
	splitchar = newchar;
}

bool	Uri::hasPort() const{
	return (existPort);
}

bool	Uri::hasPath() const{
	return (existPath);
}

unsigned short	Uri::getPort() const{
	return (port);
}

bool	Uri::hasRelativeReference() const{

	return (scheme.empty());
}

bool	Uri::ContainsRelativePath() const{
	if (path.empty())
		return true;
	else
		return (!path[0].empty());
}

void Uri::setScheme(const std::string & newScheme){
	scheme = newScheme;
}
void Uri::setHost(const std::string & newHost){
	host = newHost;
}
void Uri::setQuery(const std::string & newQuery){
	query = newQuery;
}

std::string	Uri::generateString(){
	std::ostringstream	buffer;

	if (!scheme.empty())
		buffer << scheme << ":";
	if (!host.empty())
		buffer << "//" << host;
	if (!path.empty())
	{
		if (path.size() == 1)
		{
			buffer << "/";
		}
		else
		{
			for (std::vector<std::string>::iterator it = path.begin() + 1; it != path.end(); ++it)
			{
				buffer << "/" << *it;
			}
		}
	}
	if (!query.empty())
		buffer << "?" << query;
	if (!fragment.empty())
		buffer << "#" << fragment;

	return (buffer.str());
}

bool	Uri::operator==(Uri const &rhs) const {
	if (existPath != rhs.existPath)
		return (false);
	if (existPort != rhs.existPort)
		return (false);
	if (port != rhs.port)
		return (false);
	if (scheme != rhs.scheme)
		return (false);
	if (fragment != rhs.fragment)
		return (false);
	if (query != rhs.query)
		return (false);
	if (host != rhs.host)
		return (false);
	if (path != rhs.path)
		return (false);
	return (true);
}

std::string	Uri::constructPath(){
	std::string result;

	result = "";
	if (existPath){
		for (std::vector<std::string>::iterator it = path.begin(); it != path.end(); ++it)
		{
			result += "/" + *it;
		}
	}
	return (result);
}



std::ostream & operator<<(std::ostream & o, Uri & rhs){
	(void) rhs;
	(void) o;
    return (o);
}
