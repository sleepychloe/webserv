/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 10:36:10 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/05 22:57:43 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H
# include "Colors.hpp"
// # include <string>
# include "Server.hpp"
# define FAILED RED

// class Server;
class Log
{
private:
public:
	Log();
	~Log();
	Log(Log const & src);
	Log &operator=(Log const &rhs);
	void	printInit();
	void	printServerCreation(bool success, Server *server);
	void	printConnection(std::string clientHost, unsigned int clientFd);
	void	printRequest(unsigned int clientFd, std::string method, std::string uri);
	void	printResponse(unsigned int clientFd, int statusCode);
	void	printError(std::string errorMessage);
	void	printMsg(std::string errorMessage);
};

#endif