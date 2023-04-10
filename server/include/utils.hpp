/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 14:13:50 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/07 21:26:49 by hyujung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

std::string intToString(int a);
std::size_t	vecFind(std::vector<char> rawRequest, std::string str);
std::string	vecSubstr(std::vector<char> rawRequest, size_t start, size_t end);
std::string	check_filename_get_str(const char *filename);
std::string	check_filename_get_str2(const char *filename, int *code);
void	printData(std::vector<char> data);
int	isDirectory(const char *path);
int	CheckPermission(std::string str);

#endif
