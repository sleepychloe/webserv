/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 10:11:52 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/05 22:58:20 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Log.hpp"
# include <ctime>
# include <iostream>
# include <ostream>

Log::Log(){}

Log::~Log(){}

Log::Log(Log const & src){*this = src;}

Log & Log::operator=(Log const &rhs){(void)rhs; return (*this);}

void	printTime(std::string COLOR)
{
	time_t now = std::time(0);
	tm *ltm = localtime(&now);

	std::cout << COLOR << "[" << 1900 + ltm->tm_year << "-" <<
	1 + ltm->tm_mon << "-" << ltm->tm_mday << " " <<
	ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "] ";
}
void	Log::printInit()
{
	printTime(BLUE);
	std::cout << "Initializeing Servers..." << RESET << std::endl;
}

void	Log::printServerCreation(bool success, Server *serv)
{
	if (success)
	{
		printTime(BLUE);
		std::cout << "Server Created : Host["<< serv->host << "] Port[" << serv->port << "] AssignedFd[" << serv->sockfd << "]"<< RESET <<std::endl; 
	}
	else
	{
		printTime(FAILED);
		std::cout << "Server Creation Failed : Host["<< serv->host << "] Port[" << serv->port << "]" << RESET<< std::endl; 
	}

}

void	Log::printConnection(std::string clientHost, unsigned int clientFd)
{
	printTime(PURPLE);
	std::cout << "New Connection : Host[" << clientHost <<"] AssignedFd[" << clientFd << "]" << RESET << std::endl;
}

void	Log::printRequest(unsigned int clientFd, std::string method, std::string uri)
{
	printTime(BOLD_GREEN);
	std::cout << "Request Received : Socket[" << clientFd << "] Methode=<" << method << "> URI=<" << uri << ">" << RESET << std::endl;	
}

void	Log::printResponse(unsigned int clientFd, int statusCode){
	printTime(BOLD_CYAN);
	std::cout << "Response sent to : Socket[" << clientFd << "] Status=<" << intToString(statusCode) << ">" << RESET << std::endl;	
}

void	Log::printError(std::string errorMessage){
	std::cerr << FAILED << "Error : " << errorMessage << RESET << std::endl;		
}

void	Log::printMsg(std::string msg){
	std::cout << PURPLE << msg << RESET << std::endl;		
}
