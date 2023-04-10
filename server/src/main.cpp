
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 16:44:30 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/04 18:11:49 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/ServerManager.hpp"
#include <csignal>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../config/incs/Structure.h"

int	server_main(t_http_block *info)
{
	ServerManager	sm;
	
	//config Start

	sm.setRoot(info->root);
	if (info->autoindex == "on")
		sm.setAutoIndex(true);
	else
		sm.setAutoIndex(false);
	sm.setDefaultErrorPage(info->default_error_page);
	sm.setServerBlockCount(info->server_block.size());

	std::vector<t_server_block>::iterator	iter = info->server_block.begin();

	// iterate number of server block
	for (unsigned int i = 0; i < sm.getServerBlockCount(); i++)
	{
		// add server and set param
		sm.addServerBlock();
		sm.servers[i]->setPort((*iter).listen);
		sm.servers[i]->setHost((*iter).host);
		sm.servers[i]->setMaxClientBodySize((*iter).client_max_body_size);
		sm.servers[i]->setIndex((*iter).index);
		sm.servers[i]->setUploadPath((*iter).save_path);

		std::set<std::string> method;
		if ((*iter).allow_methods_get)
			method.insert("GET");
		if ((*iter).allow_methods_post)
			method.insert("POST");
		//if ((*iter).allow_methods_delete)
		method.insert("DELETE");
		//UNKNOWN??
		sm.servers[i]->setAllowedMethod(method);

		sm.servers[i]->hasCgiBlock = (*iter).cgi_location_block.size();
		std::vector<t_cgi>::iterator		iter_cgi = (*iter).cgi_location_block.begin();
		// if there is add cgiBlock and set param
		if (sm.servers.at(i)->hasCgiBlock)
		{
			sm.servers.at(i)->setCgiPath((*iter_cgi).cgi_path);
			sm.servers.at(i)->setCgiScriptPath((*iter_cgi).path);
			std::set<std::string> ex;
			if ((*iter_cgi).cgi_extention)
			{
				sm.servers.at(i)->setCgiExt(ex);
				ex.insert(".php");
			}
		}

		sm.servers.at(i)->setLocBlockCount((*iter).index_location_block.size());
		std::vector<t_index>::iterator		iter_index = (*iter).index_location_block.begin();
		// if there is add cgiBlock and set param
		for (unsigned int j = 0; j < sm.servers.at(i)->getLocBlockCount(); j++)
		{
			sm.servers.at(i)->addLocBlock((*iter_index).path, (*iter_index).index);
			iter_index++;
		}

		sm.servers.at(i)->setRedirectBlockCount((*iter).redirection_location_block.size());
		std::vector<t_redirection>::iterator	iter_redirection = (*iter).redirection_location_block.begin();
		for (unsigned int k = 0; k < sm.servers.at(i)->getRedirectBlockCount(); k++)
		{
			sm.servers.at(i)->addRedirectBlock((*iter_redirection).path, (*iter_redirection).return_value);
			iter_redirection++;
		}

		iter++;
	}

//config End

	if (!sm.initiate())
		return (EXIT_FAILURE);
	if (sm.run())
	{
		sm.closeAndFreeMem();
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
