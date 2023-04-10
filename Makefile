# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hyujung <hyujung@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/05 12:47:08 by yhwang            #+#    #+#              #
#    Updated: 2023/04/07 21:24:51 by hyujung          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -f

CONFIG = main \
		Parse \
		HttpBlockParse \
		ServerBlockParse \
		LocationBlockParse \
		ParseUtils

SERVER = main \
		Client \
		Connection \
		MessageHeaders \
		Server \
		ServerManager \
		Uri \
		Response \
		Request \
		Cgi \
		utils \
		Log

INCS = ./Webserv.hpp

SRCS = main.cpp \
	$(addprefix ./config/srcs/, $(addsuffix .cpp, $(CONFIG))) \
	$(addprefix ./server/src/, $(addsuffix .cpp, $(SERVER)))

OBJS = main.o \
	$(addprefix ./config/srcs/, $(addsuffix .o, $(CONFIG))) \
	$(addprefix ./server/src/, $(addsuffix .o, $(SERVER)))

DEPS = $(addprefix ./server/src/, $(addsuffix .d, $(SERVER)))

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCS)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -I$(INCS)

clean:
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include ${DEPS}