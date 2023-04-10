/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 11:25:04 by hyunah            #+#    #+#             */
/*   Updated: 2023/03/06 11:25:53 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_H
# define CONNECTION_H

# include <string>
# include <vector>
# include "Client.hpp"
// # include "Server.hpp"
// # include "ServerManager.hpp"
# include <set>
# include <csignal>
# include "Request.hpp"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>

typedef void(*DataReceivedDelegate)(std::vector<unsigned char>);
typedef void(*BrokenDelegate)();

class ServerManager;
class Server;
class Response;

class Connection
{
public:

	Connection();
	~Connection();
	Connection(int socketfd);
	Connection(Connection const &src);
	Connection &operator=(Connection const &rhs);

	std::vector<char>	constructResponse();
	void				readRequest(const int &i, ServerManager *servManag);
	void				writeResponse(const int &i, ServerManager *servManag);
	void				cgiRequest(const int &i, ServerManager *servManag);
	void				cgiResponse(const int &i, ServerManager *servManag);
	std::vector<char>	dataReceived;
	std::vector<char>	dataResponse;
	struct sockaddr_in	clientAddr;
	Server				*serv;
	int					cgiState;

private:
	int		socketfd;
	Request	request;
};

#endif
