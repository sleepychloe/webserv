/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHeaders.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 08:03:47 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/05 11:16:45 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEHEADERS_H
# define MESSAGEHEADERS_H
# include <string>
# include <vector>
# include <iostream>
# include <sstream>

class MessageHeaders
{
public:

	struct	Header{
		std::string	name;
		std::string	value;
	};

	typedef		std::vector<Header> Headers;

	bool		parseFromString(const std::string &rawMsg);
	bool		parseFromString(const std::string &rawMsg, size_t & bodyOffset);
	Headers 	getHeaders() const;
	std::string	getHeaderValue(std::string name) const;
	void		setHeaderValue(std::string name, std::string value);
	bool		hasHeader(const std::string &name) const;
	std::string	getBody() const;
	void		setBody(const std::string newBody);
	std::string	generateRawMsg();
	void		addHeader(std::string name, std::string value);

	MessageHeaders();
	~MessageHeaders();
	MessageHeaders(MessageHeaders const &src);
	MessageHeaders &operator=(MessageHeaders const &rhs);

private:
	Headers		headers;
	std::string	body;
};
std::ostream & operator<<(std::ostream &output, MessageHeaders & rhs);

#endif
