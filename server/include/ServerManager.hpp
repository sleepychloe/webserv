/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 08:52:05 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/07 21:51:10 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_H
# define SERVERMANAGER_H
# include <vector>
# include <map>
# include <sys/select.h>
# include "Server.hpp"
# include "Log.hpp"

class Connection;

class ServerManager
{
private:

	fd_set					currentSockets;
	fd_set					readySockets;
	bool					error;
	int						servBlockCount;
	int						max_socket_so_far;
	std::string				commonRoot;
	std::string				commonDefaultErrorPage;
	bool					commonAutoIndex;
	std::vector<int>		serverFds;

public:
	ServerManager();
	~ServerManager();
	ServerManager(ServerManager const & src);
	ServerManager &operator=(ServerManager const & rhs);
	
	Log		log;
	fd_set					readSockets;
	fd_set					writeSockets;
	
	bool	initiate();
	bool	run();

	std::vector<Server *>	servers;
	std::map<int, Server *>	serversMap;
	std::map<int, Connection *>	connections;
	void			setServerBlockCount(int servBlockCount);
	unsigned int	getServerBlockCount(void) const;
	void			setCommonParameter(std::string root, bool autoindex, std::string defaultErrorPage);
	void			setRoot(std::string root);
	void			setAutoIndex(bool autoindex);
	void			setDefaultErrorPage(std::string dir);

	int				closeAndFreeMem();
	void			addServerBlock();
	void			addToSet(const int i, fd_set &set);
	void			removeFromSet(const int i, fd_set &set);
	void			closeConnection(const int i);
};


#endif