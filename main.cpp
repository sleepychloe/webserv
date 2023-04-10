/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 13:01:06 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/05 14:10:51 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Webserv.h"

int	main(int argc, char **argv)
{
	t_http_block	info;

	if (config_main(argc, argv, &info))
		return (1);
	server_main(&info);
	return (0);
}
