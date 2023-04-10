/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefineErrMsg.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 01:27:09 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/07 09:04:41 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_MSG_H
# define DEFINE_MSG_H

/* file open error */
# define FILE_OPEN_ERROR				99

/* http block error: 1 ~ 19 */

# define HTTP_BRAKET_OPEN				1
# define HTTP_BRAKET_CLOSE				2
# define HTTP_SEMICOLON					3
# define HTTP_INVALID_KWD				4

# define HTTP_KWD_HTTP					5

# define HTTP_KWD_ROOT					6
# define HTTP_KWD_AUTOINDEX				7
# define HTTP_KWD_DEFAULT_ERROR_PAGE			8

# define HTTP_KWD_ROOT_VALUE				9
# define HTTP_KWD_DEFAULT_ERROR_PAGE_VALUE		10

# define HTTP_KWD_ROOT_EXISTS				11
# define HTTP_KWD_AUTOINDEX_EXISTS			12
# define HTTP_KWD_DEFAULT_ERROR_PAGE_EXISTS		13

# define HTTP_KWD_HTTP_MISSED				14
# define HTTP_KWD_ROOT_MISSED				15
# define HTTP_KWD_AUTOINDEX_MISSED			16
# define HTTP_KWD_DEFAULT_ERROR_PAGE_MISSED		17

/* server block error: 20 ~ 49 */

# define SERVER_BRAKET_OPEN				20
# define SERVER_BRAKET_CLOSE				21
# define SERVER_SEMICOLON				22
# define SERVER_INVALID_KWD				23

# define SERVER_KWD_SERVER				24

# define SERVER_KWD_LISTEN				25
# define SERVER_KWD_HOST				26
# define SERVER_KWD_CLIENT_MAX_BODY_SIZE		27
# define SERVER_KWD_INDEX				28
# define SERVER_KWD_ALLOW_METHODS			29
# define SERVER_KWD_SAVE_PATH				30

# define SERVER_KWD_LISTEN_VALUE			31
# define SERVER_KWD_HOST_VALUE				32
# define SERVER_KWD_CLIENT_MAX_BODY_SIZE_VALUE		33
# define SERVER_KWD_INDEX_VALUE				34
# define SERVER_KWD_SAVE_PATH_VALUE			35

# define SERVER_KWD_LISTEN_EXISTS			36
# define SERVER_KWD_HOST_EXISTS				37
# define SERVER_KWD_CLIENT_MAX_BODY_SIZE_EXISTS		38
# define SERVER_KWD_INDEX_EXISTS			39
# define SERVER_KWD_ALLOW_METHODS_EXISTS		40
# define SERVER_KWD_SAVE_PATH_EXISTS			41

# define SERVER_KWD_SERVER_MISSED			42
# define SERVER_KWD_LISTEN_MISSED			43
# define SERVER_KWD_HOST_MISSED				44
# define SERVER_KWD_CLIENT_MAX_BODY_SIZE_MISSED		45
# define SERVER_KWD_INDEX_MISSED			46
# define SERVER_KWD_ALLOW_METHODS_MISSED		47
# define SERVER_KWD_SAVE_PATH_MISSED			48

/* location block error: 50 ~ 79 */

# define LOCATION_BRAKET_OPEN				50
# define LOCATION_BRAKET_CLOSE				51
# define LOCATION_SEMICOLON				52
# define LOCATION_INVALID_KWD				53

# define LOCATION_KWD_LOCATION				54

# define LOCATION_KWD_REDIRECTION_LOCATION		55
# define LOCATION_KWD_RETURN				56
# define LOCATION_KWD_RETURN_VALUE			57
# define LOCATION_KWD_INDEX_LOCATION			58
# define LOCATION_KWD_INDEX				59
# define LOCATION_KWD_CGI_LOCATION			60
# define LOCATION_KWD_CGI_PATH				61
# define LOCATION_KWD_CGI_EXTENTION			62

# define LOCATION_REDIRECTION_LOCATION_PATH		63
# define LOCATION_INDEX_LOCATION_PATH			64
# define LOCATION_CGI_LOCATION_PATH			65
# define LOCATION_KWD_INDEX_VALUE			66

# define LOCATION_KWD_RETURN_EXISTS			67
# define LOCATION_KWD_INDEX_EXISTS			68
# define LOCATION_KWD_CGI_LOCATION_EXISTS		69
# define LOCATION_KWD_CGI_PATH_EXISTS			70	
# define LOCATION_KWD_CGI_EXTENTION_EXISTS		71

# define LOCATION_KWD_RETURN_MISSED			72
# define LOCATION_KWD_INDEX_MISSED			73
# define LOCATION_KWD_CGI_LOCATION_MISSED		74
# define LOCATION_KWD_CGI_PATH_MISSED			75	
# define LOCATION_KWD_CGI_EXTENTION_MISSED		76

#endif
