/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 13:54:20 by yhwang            #+#    #+#             */
/*   Updated: 2023/04/05 13:16:51 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include "./HttpBlockParse.hpp"
#include "./ServerBlockParse.hpp"
#include "./LocationBlockParse.hpp"
#include "./Structure.h"
#include "./ParseUtils.h"
#include "./DefineErrMsg.h"

class	Parse: public LocationBlockParse
{
public:
	Parse();
	Parse(std::string config_file_name);
	Parse(const Parse& parse);
	Parse& operator=(const Parse& parse);
	~Parse();

	t_http_block			GetHttpBlock(void) const;

private:
	int				FileOpen(std::string config_file_name);
	void				PrintHttpBlockInfo(void);

	t_http_block			_http_block;
};

#endif
