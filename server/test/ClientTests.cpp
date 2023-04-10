/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientTests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:43:44 by hyunah            #+#    #+#             */
/*   Updated: 2023/03/09 07:55:25 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include "../include/Client.hpp"

TEST(ClientTests, ParseGetResponse){
	Client	client;
	Response *response =  client.parseResponse(
     "HTTP/1.1 200 OK\r\n"
     "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
     "Server: Apache\r\n"
     "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
     "ETag: \"34aa387-d-1568eb00\"\r\n"
     "Accept-Ranges: bytes\r\n"
     "Content-Length: 51\r\n"
     "Vary: Accept-Encoding\r\n"
     "Content-Type: text/plain\r\n"
	 "\r\n"
     "Hello World! My payload includes a trailing CRLF.\r\n"
	);
	ASSERT_FALSE(response == NULL);
	ASSERT_EQ(200, response->statusCode);
	ASSERT_EQ("OK", response->reasonPhrase);
	Uri	expectedUri;
	// expectedUri.parsingFromString("/hello.txt");
	// ASSERT_EQ(expectedUri, response->target);
	ASSERT_TRUE(response->headers.hasHeader("Date"));
	ASSERT_EQ("Mon, 27 Jul 2009 12:28:53 GMT", response->headers.getHeaderValue("Date"));
	ASSERT_TRUE(response->headers.hasHeader("Accept-Ranges"));
	ASSERT_EQ("bytes", response->headers.getHeaderValue("Accept-Ranges"));
	ASSERT_TRUE(response->headers.hasHeader("Content-Type"));
	ASSERT_EQ("text/plain", response->headers.getHeaderValue("Content-Type"));
	ASSERT_EQ(response->headers.getBody(), "Hello World! My payload includes a trailing CRLF.\r\n");
}

TEST(ClientTests, ParseIncompleteBodyRequest){
	Client	client;
	std::string	rawRequest = (
     "HTTP/1.1 200 OK\r\n"
     "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
     "Server: Apache\r\n"
     "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
     "ETag: \"34aa387-d-1568eb00\"\r\n"
     "Accept-Ranges: bytes\r\n"
     "Content-Length: 52\r\n"
     "Vary: Accept-Encoding\r\n"
     "Content-Type: text/plain\r\n"
	 "\r\n"
     "Hello World! My payload includes a trailing CRLF.\r\n"
	);
	Response *response =  client.parseResponse(rawRequest);
	ASSERT_TRUE(response == NULL);
}

// TEST(ClientTests, ParseIncompleteHeadersBetweenLinesRequest){
// 	Client	client;
// 	std::string	rawRequest = (
//      "HTTP/1.1 200 OK\r\n"
//      "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
//      "Server: Apache\r\n"
//      "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
//      "ETag: \"34aa387-d-1568eb00\"\r\n"
// 	);
// 	Response *response =  client.parseResponse(rawRequest);
// 	ASSERT_TRUE(response == NULL);
// }

// TEST(ClientTests, ParseIncompleteHeadersMidLinesRequest){
// 	Client	client;
// 	std::string	rawRequest = (
//      "HTTP/1.1 200 OK\r\n"
//      "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
//      "Server: Apache\r\n"
//      "Last-Modified: Wed, 22 Jul 2009"
// 	);
// 	Response *response =  client.parseResponse(rawRequest);
// 	ASSERT_TRUE(response == NULL);
// }

// TEST(ClientTests, ParseIncompleteStatusLinesRequest){
// 	Client	client;
// 	std::string	rawRequest = (
//      "HTTP/1.1 200 OK\r"
// 	);
// 	Response *response =  client.parseResponse(rawRequest);
// 	ASSERT_TRUE(response == NULL);
// }

// TEST(ClientTests, ParseNoHeaderRequest){
// 	Client	client;
// 	std::string	rawRequest = (
//      "HTTP/1.1 200 OK\r\n"
// 	);
// 	Response *response =  client.parseResponse(rawRequest);
// 	ASSERT_TRUE(response == NULL);
// }
