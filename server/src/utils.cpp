/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 14:11:11 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/07 22:02:51 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <vector>
# include <fstream>
# include <iostream>
#include <cstring>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

std::string intToString(int a)
{
	std::stringstream	ss;

    ss << a;
    return ss.str();
}

std::size_t	vecFind(std::vector<char> rawRequest, std::string str)
{
	size_t	i = 0;
	size_t	v = 0;

	for (std::vector<char>::iterator it = rawRequest.begin(); it != rawRequest.end(); ++it)
	{
		i = 0;
		if (*it == str[i])
		{
			while (*(it + i) == str[i])
			{
				// printf("Comparing vector char %c with str %c\n", *(it + 1), str[i]);
				// printf("i is %li, str.length is %li, returning %li\n", i, str.length()-1, v);
				if (i == str.length() - 1)
				{
					// printf("returning %li\n", v);
					return (v);
				}
				i++;
			}
		}
		v++;
	}
	return (std::string::npos);
}

std::string	vecSubstr(std::vector<char> rawRequest, size_t start, size_t end)
{
	size_t	i = 0;
	size_t	v = 0;
	std::string	ret;

	for (std::vector<char>::iterator it = rawRequest.begin(); it != rawRequest.end(); ++it)
	{
		if (v == start)
		{
			i = 0;
			while (i != end - start)
			{
				ret += *(it + i);
				i++;
			}
		}
		v++;
	}
	return (ret);
}

std::string	check_filename_get_str(const char *filename)
{
	std::string		src;
	std::string		null;
	std::string		buffer;
	std::ifstream	ifs;
	char	c;
	
	ifs.open(filename);
	if (!ifs)
	{
		std::cout << filename <<"File non-existance or Right Denied!" << std::endl;
		return (null);
	}
	while (ifs.get(c))
		src+= c;
	ifs.close();
	if (src.empty())
	{
		std::cout << "File is empty!" << std::endl;
		return (null);
	}
	return (src);
}

std::string	check_filename_get_str2(const char *filename, int *code)
{
	std::string		src;
	std::string		null;
	std::string		buffer;
	std::ifstream	ifs;
	char	c;
	
	ifs.open(filename);
	if (!ifs)
	{
		std::cout << filename <<"File non-existance or Right Denied!" << std::endl;
		*code = 0;
		return (null);
	}
	while (ifs.get(c))
		src+= c;
	ifs.close();
	if (src.empty())
	{
		std::cout << "File is empty!" << std::endl;
		*code = 1;
		return (null);
	}
	return (src);
}


void	printData(std::vector<char> data){
	printf("Start\n");
	for (std::vector<char>::iterator it = data.begin(); it != data.end(); ++it)
	{
		std::cout << *it;
	}
	printf("End\n");	
}

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

int	CheckPermission(std::string str)
{
	DIR			*dir_ptr = NULL;
	struct dirent		*file = NULL;
	std::string		token[5] = {"", };
	std::stringstream	ss(str);
	int			i = 0;
	int			flag = 0;

	while (!ss.eof())
	{
		getline(ss, token[i], '/');
		i++;
	}
	i--;

	if (i == 0)
		return (0);
	dir_ptr = opendir(token[0].append("/").append(token[1]).c_str());
	
	if (dir_ptr == NULL)
		return (0);
	
	while ((file = readdir(dir_ptr)) != NULL)
	{
		if (!strncmp(token[i].c_str(), file->d_name, strlen(token[i].c_str())))
			flag++;
	}
	closedir(dir_ptr);
	if (flag == 0)
		return (0);
	std::ifstream		file_open_check;
	file_open_check.open(str.c_str());
	if (file_open_check.fail())
		return (1);
	file_open_check.close();
	return (0);
}
