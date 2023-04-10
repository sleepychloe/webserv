/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 11:25:04 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/07 19:05:21 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H
# include "Request.hpp"
# include "Connection.hpp"
# include <string>
# include <set>
# include <csignal>
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <map>
# include <cstddef>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/stat.h>
# include <fstream>
# define BUFFSIZE 2048

class ServerManager;
class Server
{
public:
	Server();
	~Server();
	Server(Server const &src);
	Server &operator=(Server const &rhs);

	struct LocationBlock{
		std::string	dir;
		std::string	index;};

	struct CgiBlock{
		std::string	cgiPath;
		std::string	cgiScriptPath;
		std::set<std::string>	cgiExt;};

	struct RedirectBlock{
		std::string	dir;
		std::string	ret;
		};

	int					startListen();
	int					acceptConnection(ServerManager *servManag);
	void				newConnection();
	void				writeResponse(const int &i);
	std::string			findMatchingUri(std::string path);

	void				setLocBlockCount(unsigned int i);
	void				setRedirectBlockCount(unsigned int i);

	void				setPort(unsigned short i);
	void				setUploadPath(std::string path);
	void				setHost(std::string str);
	void				setMaxClientBodySize(unsigned int i);
	void				setIndex(std::string index);
	void				setAllowedMethod(std::set<std::string> m);

	void				setCgiPath(std::string path);
	void				setCgiExt(std::set<std::string> extension);
	void				setCgiScriptPath(std::string path);
	void				cgiRequest();
	void				cgiResponse();
	unsigned int		getLocBlockCount();
	unsigned int		getRedirectBlockCount();
	void				addLocBlock(std::string dir, std::string index);
	void				addRedirectBlock(std::string dir, std::string ret);

	int						port;
	int						sockfd;
	bool					autoIndex;
	bool					hasCgiBlock;
	// int						clientfd;
	int						cgiState;
	int						maxClientBodySize;
	std::string				uploadPath;
	std::string				error_page;
	std::string				host;
	std::string				root;
	std::string				index;
	std::set<std::string>	allowedMethod;
	void					*manager;
	std::set<LocationBlock *>	locationBloc;
	std::set<RedirectBlock *>	redirectionBloc;
	CgiBlock				cgiBloc;
	// Request					request;
private:
    // std::vector<char>		data;
	struct sockaddr_in		serverAddr;
	// struct sockaddr_in		clientAddr;
	unsigned int			nLoc;
	unsigned int			nRedirect;
};

#endif
