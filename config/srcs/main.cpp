/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 13:56:00 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/05 13:59:47 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Parse.hpp"

int	config_main(int argc, char **argv, t_http_block *info)
{
	/* error check: argument */
	if (!(argc == 1 || argc == 2))
	{
		std::cerr << _RED << "Error: Argument error" << _BLACK << std::endl
			<< "(command: " << _CYAN << "./parse" << _BLACK << " or "
			<< _CYAN << "./parse config_file_name" << _BLACK ")" << std::endl;
		return (1);
	}

	try
	{
		if (argc == 1)
		{
			Parse	p("conf_files/default.conf");
			*info = p.GetHttpBlock();
		}
		else
		{
			Parse	p(argv[1]);
			*info = p.GetHttpBlock();
		}
	}
	catch(std::string err_msg)
	{
		std::cerr << err_msg << std::endl;
		return (1);
	}
	return (0);
}
