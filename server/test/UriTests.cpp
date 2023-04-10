/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UriTests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 15:43:44 by hyunah            #+#    #+#             */
/*   Updated: 2023/03/21 10:40:30 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include "../include/Uri.hpp"


//TO DO : 
// CHECK INVALID INPUT

TEST(UriTests, ParseFromString){
	Uri uri;
	ASSERT_TRUE(uri.parsingFromString("http://www.example.com/foo/bar"));
	ASSERT_EQ("http", uri.getScheme());
	ASSERT_EQ("www.example.com", uri.getHost());
	ASSERT_EQ(
		(std::vector<std::string>{
			"", 
			"foo", 
			"bar",
			}), 
			uri.getPath()
	);
	uri.setSplitChar(":");
	ASSERT_TRUE(uri.parsingFromString("urn:book:fantasy:Hobbit"));
	ASSERT_EQ("urn", uri.getScheme());
	ASSERT_EQ("", uri.getHost());
	ASSERT_EQ(
		(std::vector<std::string>{
			"book", 
			"fantasy", 
			"Hobbit",
			}), 
			uri.getPath()
	);
}

TEST(UriTests, ParseFromStringNoScheme){
	Uri uri;
	ASSERT_TRUE(uri.parsingFromString("foo/bar"));
	ASSERT_EQ("", uri.getScheme());
	ASSERT_EQ(
		(std::vector<std::string>{
			"foo", 
			"bar",
			}), 
			uri.getPath()
	);
}

TEST(UriTests, ParseFromStringEndsAfterAuthority){
	Uri uri;
	ASSERT_TRUE(uri.parsingFromString("http://www.example.com"));
	ASSERT_EQ("http", uri.getScheme());
	ASSERT_EQ("www.example.com", uri.getHost());
}

TEST(UriTests, ParseFromStringNoPath){
	Uri uri;
	ASSERT_TRUE(uri.parsingFromString("http://www.example.com"));
	ASSERT_EQ("http", uri.getScheme());
	ASSERT_EQ("www.example.com", uri.getHost());
	ASSERT_EQ(
		(std::vector<std::string>{}), 
			uri.getPath()
	);
}
TEST(UriTests, ParseFromStringMultiCharacter){
	Uri uri;

	uri.setSplitChar("/-");
	ASSERT_TRUE(uri.parsingFromString("urn:book/-fant/asy/-Hobb-it"));
	ASSERT_EQ("urn", uri.getScheme());
	ASSERT_EQ("", uri.getHost());
	ASSERT_EQ(
		(std::vector<std::string>{
			"book", 
			"fant/asy", 
			"Hobb-it",
			}), 
			uri.getPath()
	);
}

TEST(UriTests, ParseFromStringPathCornerCases){
	// "/" -> [""]
	// ""  -> []
	// "foo/"  -> ["foo", ""]
	// "/foo"  -> ["", "foo"]	
	Uri uri;

	uri.setSplitChar("/");
	ASSERT_TRUE(uri.parsingFromString("/"));
	ASSERT_EQ((std::vector<std::string> {""}), uri.getPath());

	ASSERT_TRUE(uri.parsingFromString(""));
	ASSERT_EQ((std::vector<std::string> {}), uri.getPath());

	ASSERT_TRUE(uri.parsingFromString("foo/"));
	ASSERT_EQ((std::vector<std::string> {"foo"}), uri.getPath());

	ASSERT_TRUE(uri.parsingFromString("/foo"));
	ASSERT_EQ((std::vector<std::string> {"", "foo"}), uri.getPath());
}

TEST(UriTests, ParseFromStringHasPortNumber){
	Uri uri;

	ASSERT_TRUE(uri.parsingFromString("http://www.lolo.com:8080/foo/bar"));
	ASSERT_EQ("www.lolo.com", uri.getHost());
	ASSERT_TRUE(uri.hasPort());
	ASSERT_EQ(8080, uri.getPort());
}

TEST(UriTests, ParseFromStringDoesNotPortNumber){
	Uri uri;

	ASSERT_TRUE(uri.parsingFromString("http://www.lolo.com/foo/bar"));
	ASSERT_EQ("www.lolo.com", uri.getHost());
	ASSERT_FALSE(uri.hasPort());
}

TEST(UriTests, ParseFromStringTwiceFirstWithPortNumberThenWithout){
	Uri uri;

	ASSERT_TRUE(uri.parsingFromString("http://www.lolo.com:8000/foo/bar"));
	ASSERT_TRUE(uri.parsingFromString("http://www.lolo.com/foo/bar"));
	ASSERT_FALSE(uri.hasPort());
}

TEST(UriTests, ParseFromStringFirstPortThenNoPort){
	Uri uri;

	ASSERT_TRUE(uri.parsingFromString("http://www.lolo.com:8080/foo/bar"));
	ASSERT_EQ("www.lolo.com", uri.getHost());
	ASSERT_TRUE(uri.hasPort());
	ASSERT_EQ(8080, uri.getPort());
	ASSERT_TRUE(uri.parsingFromString("http://www.lolo.com/foo/bar"));
	ASSERT_EQ("www.lolo.com", uri.getHost());
	ASSERT_FALSE(uri.hasPort());
}

TEST(UriTests, ParseFromStringBadPortNumber){
	Uri uri;

	ASSERT_FALSE(uri.parsingFromString("http://www.lolo.com:spam/foo/bar"));
	ASSERT_FALSE(uri.parsingFromString("http://www.lolo.com:spam8080/foo/bar"));
	ASSERT_FALSE(uri.parsingFromString("http://www.lolo.com:8080spam/foo/bar"));
	ASSERT_FALSE(uri.parsingFromString("http://www.lolo.com:99999999999999/foo/bar"));
	ASSERT_FALSE(uri.parsingFromString("http://www.lolo.com:-1/foo/bar"));
	ASSERT_TRUE(uri.parsingFromString("http://www.lolo.com:65535/foo/bar"));
	ASSERT_TRUE(uri.parsingFromString("http://www.lolo.com:0/foo/bar"));
}

TEST(UriTests, ParseFromStringIsRelativeReference){
	Uri uri;
	//No scheme
	ASSERT_TRUE(uri.parsingFromString("http://www.example.com/"));
	ASSERT_EQ(uri.hasRelativeReference(), false);

	ASSERT_TRUE(uri.parsingFromString("http://www.example.com"));
	ASSERT_EQ(uri.hasRelativeReference(), false);

	ASSERT_TRUE(uri.parsingFromString("/"));
	ASSERT_EQ(uri.hasRelativeReference(), true);

	ASSERT_TRUE(uri.parsingFromString("foo"));
	ASSERT_EQ(uri.hasRelativeReference(), true);

	ASSERT_TRUE(uri.parsingFromString(""));
	ASSERT_EQ(uri.hasRelativeReference(), true);

}

TEST(UriTests, ParseFromStringIsRelativePath){
	Uri uri;

	ASSERT_TRUE(uri.parsingFromString("http://www.example.com/"));
	ASSERT_EQ(uri.ContainsRelativePath(), false);

	ASSERT_TRUE(uri.parsingFromString("http://www.example.com"));
	ASSERT_EQ(uri.ContainsRelativePath(), true);

	ASSERT_TRUE(uri.parsingFromString("/"));
	ASSERT_EQ(uri.ContainsRelativePath(), false);

	ASSERT_TRUE(uri.parsingFromString("foo"));
	ASSERT_EQ(uri.ContainsRelativePath(), true);
}

TEST(UriTests, ParseFromStringFragmentsQuerys){
	Uri uri;

	ASSERT_TRUE(uri.parsingFromString("http://www.example.com/"));
	ASSERT_EQ(uri.getFragement(), "");
	ASSERT_EQ(uri.getQuery(), "");
	ASSERT_EQ(uri.getHost(), "www.example.com");

	ASSERT_TRUE(uri.parsingFromString("http://www.example.com#foo"));
	ASSERT_EQ(uri.getFragement(), "foo");
	ASSERT_EQ(uri.getQuery(), "");
	ASSERT_EQ(uri.getHost(), "www.example.com");

	ASSERT_TRUE(uri.parsingFromString("http://www.example.com?foo"));
	ASSERT_EQ(uri.getFragement(), "");
	ASSERT_EQ(uri.getQuery(), "foo");
	ASSERT_EQ(uri.getHost(), "www.example.com");

	ASSERT_TRUE(uri.parsingFromString("http://www.example.com?foo#bar"));
	ASSERT_EQ(uri.getHost(), "www.example.com");
	ASSERT_EQ(uri.getQuery(), "foo");
	ASSERT_EQ(uri.getFragement(), "bar");

	ASSERT_TRUE(uri.parsingFromString("http://www.example.com?earth?day#bar"));
	ASSERT_EQ(uri.getHost(), "www.example.com");
	ASSERT_EQ(uri.getQuery(), "earth?day");
	ASSERT_EQ(uri.getFragement(), "bar");

	ASSERT_TRUE(uri.parsingFromString("http://www.example.com/spam?foo#bar"));
	ASSERT_EQ(uri.getHost(), "www.example.com");
	ASSERT_EQ(uri.getQuery(), "foo");
	ASSERT_EQ(uri.getFragement(), "bar");

	// TO RESEARCH : 
	// ASSERT_TRUE(uri.parsingFromString("http://www.example.com/?"));
	// ASSERT_TRUE(uri.parsingFromString("http://www.example.com/??foo"));
}

TEST(UriTests, GenerateString){
	Uri	uri, uri2;

	uri.setScheme("http");
	uri.setHost("www.example.com");
	uri.setQuery("foo");
	ASSERT_EQ(uri.generateString(), "http://www.example.com?foo");

	uri.setScheme("");
	uri.setHost("example.com");
	uri.setQuery("bar");
	ASSERT_EQ(uri.generateString(), "//example.com?bar");

	uri.setScheme("");
	uri.setHost("example.com");
	uri.setQuery("");
	ASSERT_EQ(uri.generateString(), "//example.com");

	uri.setScheme("");
	uri.setHost("");
	uri.setQuery("");
	ASSERT_EQ(uri.generateString(), "");

	uri.setScheme("");
	uri.setHost("");
	uri.setQuery("bar");
	ASSERT_EQ(uri.generateString(), "?bar");

	uri.setScheme("");
	uri.setHost("");
	uri.setQuery("bar");
	ASSERT_EQ(uri.generateString(), "?bar");

	uri.setScheme("http");
	uri.setHost("");
	uri.setQuery("bar");
	ASSERT_EQ(uri.generateString(), "http:?bar");

	uri.setScheme("http");
	uri.setHost("");
	uri.setQuery("");
	ASSERT_EQ(uri.generateString(), "http:");

	uri.setScheme("http");
	uri.setHost("1.2.3.4");
	uri.setQuery("");
	ASSERT_EQ(uri.generateString(), "http://1.2.3.4");

	uri2.parsingFromString("http://www.example.com/");
	ASSERT_EQ(uri2.generateString(), "http://www.example.com/");

	uri2.parsingFromString("http://www.example.com/bar");
	ASSERT_EQ(uri2.generateString(), "http://www.example.com/bar");

	uri2.parsingFromString("http://www.example.com/bar/foo");
	ASSERT_EQ(uri2.generateString(), "http://www.example.com/bar/foo");

}


TEST(UriTests, GenerateHeaderDisposition){
	Uri	uri;

	uri.parsingFromString("/home/hyunah/Documents/webserv/data/fruits/sampleVideo.avi");
	std::string fileDefaultFileName = "inline; filename=";
	fileDefaultFileName += "\"" + uri.getPath().back() + "\"";
	ASSERT_EQ("inline; filename=\"sampleVideo.avi\"", fileDefaultFileName);
}