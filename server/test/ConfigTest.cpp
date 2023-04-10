/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigTest.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:32:26 by hyunah            #+#    #+#             */
/*   Updated: 2023/03/14 18:28:00 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include "../include/Config.hpp"

// TEST(ConfigTests, ParseString1){
// 	Config config;

// 	std::string	rawMsg = {
// "server{\n"
// "}\n"
// "server{\n"
// "}"
// };
// 	ASSERT_TRUE(config.parseFromString(rawMsg));
// 	ASSERT_EQ(2, config.getServerCount());
// }

// TEST(ConfigTests, ParseString2){
// 	Config config;

// 	std::string	rawMsg = {
// "server{\n"
// "	listen	8000;"
// "	root /home/hyunah/Desktop/mysite;"
// "	server_name 127.0.0.1;"
// "}\n"
// "server{\n"
// "}"
// };
// 	ASSERT_TRUE(config.parseFromString(rawMsg));
// 	ASSERT_EQ(2, config.getServerCount());
// }
