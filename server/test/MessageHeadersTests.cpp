/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHeadersTests.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:43:44 by hyunah            #+#    #+#             */
/*   Updated: 2023/03/05 21:09:54 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include "../include/MessageHeaders.hpp"

//TO DO: RFC 5433, 7230, 3261. check invalidity
TEST(MessageHeadersTests, HttpClientRequestMessage){
	MessageHeaders msg;
	std::string rawMsg = (
		"User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3\r\n"
		"Host: www.example.com\r\n"
		"Accept-Language: en, mi\r\n"
		"\r\n");
	ASSERT_TRUE(msg.parseFromString(rawMsg));
	MessageHeaders::Headers headers = msg.getHeaders();
	struct ExpectedHeader {
		std::string name;
		std::string value;
	};

	std::vector<ExpectedHeader> expectedHeader{
		{"User-Agent", "curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3"},
		{"Host", "www.example.com"},
		{"Accept-Language", "en, mi"}
	};
	ASSERT_EQ(expectedHeader.size(), headers.size());
	int i = 0;
	for(std::vector<ExpectedHeader>::iterator it=expectedHeader.begin(); it != expectedHeader.end(); ++it)
	{
		ASSERT_EQ(it->name, headers[i].name);
		ASSERT_EQ(it->value, headers[i].value);
		i++;
	}
	ASSERT_TRUE(msg.hasHeader("Host"));
	ASSERT_FALSE(msg.hasHeader("Bobo"));
	ASSERT_EQ(msg.generateRawMsg(), rawMsg);
}

TEST(MessageHeadersTests, HttpServerResponseMessage){
	MessageHeaders	msg;
	size_t			bodyOffset;
	std::string rawHeader = (
     "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
     "Server: Apache\r\n"
     "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
     "ETag: \"34aa387-d-1568eb00\"\r\n"
     "Accept-Ranges: bytes\r\n"
     "Content-Length: 51\r\n"
     "Vary: Accept-Encoding\r\n"
     "Content-Type: text/plain\r\n"
	 "\r\n"
	);
	std::string rawMsg = (
	rawHeader
    + "Hello World! My payload includes a trailing CRLF.\r\n"
	);
	ASSERT_TRUE(msg.parseFromString(rawMsg, bodyOffset));
	ASSERT_EQ(rawHeader.length(), bodyOffset);
	MessageHeaders::Headers headers = msg.getHeaders();
	struct ExpectedHeader {
		std::string name;
		std::string value;
	};
	std::vector<ExpectedHeader> expectedHeader{
		{"Date", "Mon, 27 Jul 2009 12:28:53 GMT"},
		{"Server", "Apache"},
		{"Last-Modified", "Wed, 22 Jul 2009 19:15:56 GMT"},
		{"ETag", "\"34aa387-d-1568eb00\""},
		{"Accept-Ranges", "bytes"},
		{"Content-Length", "51"},
		{"Vary", "Accept-Encoding"},
		{"Content-Type", "text/plain"},
		};
	ASSERT_EQ(expectedHeader.size(), headers.size());
	int i = 0;
	for(std::vector<ExpectedHeader>::iterator it=expectedHeader.begin(); it != expectedHeader.end(); ++it)
	{
		ASSERT_EQ(it->name, headers[i].name);
		ASSERT_EQ(it->value, headers[i].value);
		i++;
	}
	ASSERT_TRUE(msg.hasHeader("Content-Length"));
	ASSERT_FALSE(msg.hasHeader("Bobo"));
	ASSERT_EQ(msg.getBody(), "Hello World! My payload includes a trailing CRLF.\r\n");
	ASSERT_EQ(msg.generateRawMsg(), rawMsg);
}
