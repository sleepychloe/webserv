/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 04:20:47 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/07 18:34:02 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/ParseUtils.h"

int	StringCheck(std::string str)
{
	for (int i = 0; i < (int)str.length(); i++)
	{
		if (!(str[i] == ' ' || str[i] == '\t'))
			return (1);
	}
	return (0);
}

int	StringCheck(std::string str, char c)
{
	for (int i = 0; i < (int)str.length(); i++)
	{
		if (!(str[i] == ' ' || str[i] == '\t' || str[i] == c))
			return (1);
	}
	return (0);
}

int	SemicolonCheck(std::string str)
{
	int	flag = 0;

	for (int i = 0; i < (int)str.length(); i++)
		if (str[i] == ';')
			flag++;
	if (flag != 1)
		return (1);
	if (!(str[str.length() - 1] == ';'))
		return (1);
	return (0);
}

int	CheckDirectory(std::string str, int flag)
{
	DIR		*dir_ptr = NULL;
	struct dirent	*file = NULL;
	int		i = 0;

	dir_ptr = opendir(str.c_str());
	if (dir_ptr == NULL)
		return (0);
	if (flag == ROOT)
	{
		while ((file = readdir(dir_ptr)) != NULL)
		{
			if (!strncmp("cgi-bin", file->d_name, strlen("cgi-bin")))
				i++;
			if (!strncmp("error_pages", file->d_name, strlen("error_pages")))
				i++;
			if (!strncmp("upload", file->d_name, strlen("upload")))
				i++;
		}
		if (i < 3)
			return (0);
	}
	if (flag == SAVE_PATH)
	{
		while ((file = readdir(dir_ptr)) != NULL)
		{
			if (!strncmp(".upload", file->d_name, strlen("cgi-bin")))
				i++;
		}
		if (i == 0)
			return (0);
	}
	if (flag == CGI_BIN_LOCATION)
	{
		while ((file = readdir(dir_ptr)) != NULL)
		{
			if (!strncmp("query.php", file->d_name, strlen("query.php")))
				i++;
			if (!strncmp("upload.php", file->d_name, strlen("upload.php")))
				i++;
		}
		if (i != 2)
			return (0);
	}
	closedir(dir_ptr);
	return (1);
}

std::string	RemoveSpaceTab(std::string str)
{
	int	start = 0;
	int	end = (int)str.length();

	for (int i = 0; i < (int)str.length(); i++)
	{
		if (str[i] == ' ' || str[i] == '\t')
			start++;
	}
	for (int i = (int)str.length(); 0 < i; i--)
	{
		if (str[i] == ' ' || str[i] == '\t')
			end--;
	}
	return (str.substr(start, end));
}

static std::string	ReplaceLine(std::string line, std::string s1, std::string s2)
{
	std::string	new_line = "";
	int		pos = 0;
	int		find = 0;

	while (1)
	{
		find = line.find(s1, pos);
		if (find == -1)
		{
			new_line += line.substr(pos, std::string::npos);
			break ;
		}
		new_line += line.substr(pos, find - pos);
		new_line += s2;
		pos += (find - pos + s1.length());
	}
	return (new_line);
}

int	TokenCount(int n, std::string line, std::string *token)
{
	int		i = 0;

	line = ReplaceLine(line, "\t", " ");
	std::stringstream	ss(line);
	while (!ss.eof())
	{
		getline(ss, token[i], ' ');
		if (token[i] == "")
			continue ;
		i++;
	}
	if (i != n)
		return (1);
	return (0);
}

int	TokenCount(std::string line, std::string *token)
{
	int		i = 0;

	line = ReplaceLine(line, "\t", " ");
	std::stringstream	ss(line);
	while (!ss.eof())
	{
		getline(ss, token[i], ' ');
		if (token[i] == "")
			continue ;
		i++;
	}
	return (i);
}

std::string	ErrMsg(std::string file_name, int err_number, std::string line, int i)
{
	std::string	msg = file_name;

	msg.append(":");

	std::stringstream	ss;
	ss << i;
	msg.append(ss.str());

	msg.append(": ");
	msg.append(_RED);
	msg.append("error: ");
	msg.append(_BLACK);

	if (err_number == 99)
	{
		msg = "";
		msg.append(_RED);
		msg.append("error: ");
		msg.append(_BLACK);
		msg.append("file open error");
		return (msg);
	}
	else if (1 <= err_number && err_number <= 19)
	{
		msg.append("http block: ");
		if (err_number == HTTP_BRAKET_OPEN)
			msg.append("'{' is missed, used multiple time, or not opened properly\n");
		else if (err_number == HTTP_BRAKET_CLOSE)
			msg.append("'}' is missed, used multiple time, or not closed properly\n");
		else if (err_number == HTTP_SEMICOLON)
			msg.append("';' is missed, used multiple time, or not used properly\n");
		else if (err_number == HTTP_INVALID_KWD)
			msg.append("found invalid character(s)\n");
		else if (err_number == HTTP_KWD_HTTP)
			msg.append("keyword http is not used properly\n");
		else if (err_number == HTTP_KWD_ROOT)
			msg.append("keyword root is not used properly\n");
		else if (err_number == HTTP_KWD_AUTOINDEX)
			msg.append("keyword autoindex is not used properly\n");
		else if (err_number == HTTP_KWD_DEFAULT_ERROR_PAGE)
			msg.append("keyword default_error_page is not used properly\n");
		else if (err_number == HTTP_KWD_ROOT_VALUE)
			msg.append("value of keyword root is invalid\n");
		else if (err_number == HTTP_KWD_DEFAULT_ERROR_PAGE_VALUE)
			msg.append("value of keyword default_error_page is invalid\n");
		else if (err_number == HTTP_KWD_ROOT_EXISTS)
			msg.append("keyword root already exists\n");
		else if (err_number == HTTP_KWD_AUTOINDEX_EXISTS)
			msg.append("keyword autoindex already exists\n");
		else if (err_number == HTTP_KWD_DEFAULT_ERROR_PAGE_EXISTS)
			msg.append("keyword default_error_page already exists\n");
		else if (err_number == HTTP_KWD_HTTP_MISSED)
		{
			msg = "";
			msg = file_name;
			msg.append(": ");
			msg.append(_RED);
			msg.append("error: ");
			msg.append(_BLACK);
			msg.append("http block is missed");
			return (msg);
		}
		else if (err_number == HTTP_KWD_ROOT_MISSED)
			msg.append("keyword root is missed\n");
		else if (err_number == HTTP_KWD_AUTOINDEX_MISSED)
			msg.append("keyword autoindex is missed\n");
		else if (err_number == HTTP_KWD_DEFAULT_ERROR_PAGE_MISSED)
			msg.append("keyword default_error_page is missed\n");
	}
	else if (20 <= err_number && err_number <= 49)
	{
		msg.append("server block: ");
		if (err_number == SERVER_BRAKET_OPEN)
			msg.append("'{' is missed, used multiple time, or not opened properly\n");
		else if (err_number == SERVER_BRAKET_CLOSE)
			msg.append("'}' is missed, used multiple time, or not closed properly\n");
		else if (err_number == SERVER_SEMICOLON)
			msg.append("';' is missed, used multiple time, or not used properly\n");
		else if (err_number == SERVER_INVALID_KWD)
			msg.append("found invalid character(s)\n");
		else if (err_number == SERVER_KWD_SERVER)
			msg.append("keyword server is not used properly\n");
		else if (err_number == SERVER_KWD_LISTEN)
			msg.append("keyword listen is not used properly\n");
		else if (err_number == SERVER_KWD_HOST)
			msg.append("keyword host is not used properly\n");
		else if (err_number == SERVER_KWD_CLIENT_MAX_BODY_SIZE)
			msg.append("keyword client_max_body_size is not used properly\n");
		else if (err_number == SERVER_KWD_INDEX)
			msg.append("keyword index is not used properly\n");
		else if (err_number == SERVER_KWD_ALLOW_METHODS)
			msg.append("keyword allow_methods is not used properly\n");
		else if (err_number == SERVER_KWD_SAVE_PATH)
			msg.append("keyword save_path is not used properly\n");
		else if (err_number == SERVER_KWD_LISTEN_VALUE)
			msg.append("value of keyword listen is invalid\n");
		else if (err_number == SERVER_KWD_HOST_VALUE)
			msg.append("value of keyword host is invalid\n");
		else if (err_number == SERVER_KWD_CLIENT_MAX_BODY_SIZE_VALUE)
			msg.append("value of keyword client_max_body_size is invalid\n");
		else if (err_number == SERVER_KWD_INDEX_VALUE)
			msg.append("value of keyword index is invalid\n");
		else if (err_number == SERVER_KWD_SAVE_PATH_VALUE)
			msg.append("value of keyword save_path is invalid\n");
		else if (err_number == SERVER_KWD_LISTEN_EXISTS)
			msg.append("keyword listen already exists\n");
		else if (err_number == SERVER_KWD_HOST_EXISTS)
			msg.append("keyword host already exists\n");
		else if (err_number == SERVER_KWD_CLIENT_MAX_BODY_SIZE_EXISTS)
			msg.append("keyword client_max_body_size already exists\n");
		else if (err_number == SERVER_KWD_INDEX_EXISTS)
			msg.append("keyword index already exists\n");
		else if (err_number == SERVER_KWD_ALLOW_METHODS_EXISTS)
			msg.append("keyword allow_methods already exists\n");
		else if (err_number == SERVER_KWD_SAVE_PATH_EXISTS)
			msg.append("keyword save_path already exists\n");
		else if (err_number == SERVER_KWD_SERVER_MISSED)
			msg.append("server block is missed\n");
		else if (err_number == SERVER_KWD_LISTEN_MISSED)
			msg.append("keyword listen is missed\n");
		else if (err_number == SERVER_KWD_HOST_MISSED)
			msg.append("keyword host is missed\n");
		else if (err_number == SERVER_KWD_CLIENT_MAX_BODY_SIZE_MISSED)
			msg.append("keyword client_max_body_size is missed\n");
		else if (err_number == SERVER_KWD_INDEX_MISSED)
			msg.append("keyword index is missed\n");
		else if (err_number == SERVER_KWD_ALLOW_METHODS_MISSED)
			msg.append("keyword allow_methods is missed\n");
		else if (err_number == SERVER_KWD_SAVE_PATH_MISSED)
			msg.append("keyword save_path is missed\n");
	}
	else if (50 <= err_number && err_number <= 79)
	{
		msg.append("location block: ");
		if (err_number == LOCATION_BRAKET_OPEN)
			msg.append("'{' is missed, used multiple time, or not opened properly\n");
		else if (err_number == LOCATION_BRAKET_CLOSE)
			msg.append("'}' is missed, used multiple time, or not closed properly\n");
		else if (err_number == LOCATION_SEMICOLON)
			msg.append("';' is missed, used multiple time, or not used properly\n");
		else if (err_number == LOCATION_INVALID_KWD)
			msg.append("found invalid character(s)\n");
		else if (err_number == LOCATION_KWD_LOCATION)
			msg.append("keyword location is not used properly\n");
		else if (err_number == LOCATION_KWD_REDIRECTION_LOCATION)
			msg.append("_REDirection location is not used properly\n");
		else if (err_number == LOCATION_KWD_RETURN)
			msg.append("keyword return is not used properly\n");
		else if (err_number == LOCATION_KWD_RETURN_VALUE)
			msg.append("return value is not used properly next to the keyword return\n");
		else if (err_number == LOCATION_KWD_INDEX_LOCATION)
			msg.append("index location is not used properly\n");
		else if (err_number == LOCATION_KWD_INDEX)
			msg.append("keyword index is not used properly\n");
		else if (err_number == LOCATION_KWD_CGI_LOCATION)
			msg.append("cgi-bin location is not used properly\n");
		else if (err_number == LOCATION_KWD_CGI_PATH)
			msg.append("keyword cgi_path is not used properly\n");
		else if (err_number == LOCATION_KWD_CGI_EXTENTION)
			msg.append("keyword cgi_extention is not used properly\n");
		
		else if (err_number == LOCATION_REDIRECTION_LOCATION_PATH)
			msg.append("path of redirection location block is invalid\n");
		else if (err_number == LOCATION_INDEX_LOCATION_PATH)
			msg.append("path of index location block is invalid\n");
		else if (err_number == LOCATION_CGI_LOCATION_PATH)
			msg.append("path of cgi-bin location block is invalid\n");
		else if (err_number == LOCATION_KWD_INDEX_VALUE)
			msg.append("value of keyword index is invalid\n");


		
		else if (err_number == LOCATION_KWD_RETURN_EXISTS)
			msg.append("keyword return already exists\n");
		else if (err_number == LOCATION_KWD_INDEX_EXISTS)
			msg.append("keyword index already exists\n");
		else if (err_number == LOCATION_KWD_CGI_LOCATION_EXISTS)
			msg.append("cgi-bin location block already exists\n");
		else if (err_number == LOCATION_KWD_CGI_PATH_EXISTS)
			msg.append("keyword cgi_path already exists\n");
		else if (err_number == LOCATION_KWD_CGI_EXTENTION_EXISTS)
			msg.append("keyword cgi_extention already exists\n");
		else if (err_number == LOCATION_KWD_RETURN_MISSED)
			msg.append("keyword return is missed\n");
		else if (err_number == LOCATION_KWD_INDEX_MISSED)
			msg.append("keyword index is missed\n");
		else if (err_number == LOCATION_KWD_CGI_LOCATION_MISSED)
			msg.append("cgi-bin location block is missed\n");
		else if (err_number == LOCATION_KWD_CGI_PATH_MISSED)
			msg.append("keyword cgi_path is missed\n");
		else if (err_number == LOCATION_KWD_CGI_EXTENTION_MISSED)
			msg.append("keyword cgi_extention is missed\n");
	}
	else
	{
		msg = "";
		msg.append(_RED);
		msg.append("error");
		msg.append(_BLACK);
		return (msg);
	}
	
	if (i < 10)
		msg.append("   ");
	else if (i < 100)
		msg.append("  ");
	else
		msg.append(" ");

	msg.append(ss.str());
	msg.append(" | ");
	
	msg.append(line);
	return (msg);
}
