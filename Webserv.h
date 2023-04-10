/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 13:16:28 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/05 14:13:32 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

#include "./config/incs/Structure.h"

int				config_main(int argc, char **argv, t_http_block *info);
int				server_main(t_http_block *info);

#endif
