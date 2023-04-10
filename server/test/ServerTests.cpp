/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerTests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 19:12:51 by hyunah            #+#    #+#             */
/*   Updated: 2023/03/17 20:49:33 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <gtest/gtest.h>
#include "../include/Server.hpp"

TEST(ServersTests, ParseGetRequest){
//config Start
	Server			server1;
	Server			server2;
	server1.port = 8002;
	server1.host = "127.0.0.1";
	server1.root = "./data";
	server1.serverName = "RatatouilleServer1";
	// server1.locationRoot = "/fruits"; //relative path ok
	server1.error_page = "/home/hyunah/Documents/webserv/data/error_pages";
	server1.maxClientBodySize = 400;
	server1.index = "index_1.html";
	server1.allowedMethod.insert("GET");
	server1.allowedMethod.insert("POST");
	Server::LocationBlock locbloc1;
	Server::LocationBlock locbloc2;
	locbloc1.hasReturn = false;
	locbloc1.isCgi_bin = false;
	locbloc1.info.insert(std::pair<std::string, std::string>("dir", "/fruits"));
	locbloc1.info.insert(std::pair<std::string, std::string>("index", "yummyfruits.html"));
	server1.locationBloc.insert(&locbloc1);

	locbloc2.hasReturn = false;
	locbloc2.isCgi_bin = false;
	locbloc2.info.insert(std::pair<std::string, std::string>("dir", "/lulu"));
	locbloc2.info.insert(std::pair<std::string, std::string>("index", "lolo.html"));
	server1.locationBloc.insert(&locbloc2);



	ASSERT_EQ(server1.findMatchingUri("/"), "./data/index_1.html");
	ASSERT_EQ(server1.findMatchingUri("/fruits"), "./data/fruits/yummyfruits.html");
	
	locbloc1.info.erase("index");
	ASSERT_EQ(server1.findMatchingUri("/fruits"), "./data/fruits/index.html");

	server1.index = "index.html";
	ASSERT_EQ(server1.findMatchingUri("/"), "./data/index.html");

	server1.root = "data";
	ASSERT_EQ(server1.findMatchingUri("/"), "data/index.html");

	ASSERT_EQ(server1.findMatchingUri("/lulu"), "data/lulu/lolo.html");

	// ASSERT_EQ(server1.findMatchingUri("/fruits/strawberry.jpg"), "./data/fruits/strawberry.jpg");
}