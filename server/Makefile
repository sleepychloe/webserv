NAME			= webserv

SRCS		=	$(addprefix src/,									\
												main.cpp			\
												Client.cpp			\
												Connection.cpp		\
												MessageHeaders.cpp	\
												Server.cpp			\
												ServerManager.cpp	\
												Uri.cpp				\
												Response.cpp		\
												Request.cpp			\
												Cgi.cpp			\
												utils.cpp			\
												Log.cpp			\
				)

OBJS			= $(SRCS:.cpp=.o)
DEPS			= $(SRCS:.cpp=.d)

CXX				= c++
RM				= rm -f
CXXFLAGS		= -MMD -Wall -Wextra -Werror -std=c++98

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS) $(DEPS)

fclean:			clean
				$(RM) $(NAME)

eval:			$(NAME)
				make clean
				reset
				valgrind ./$(NAME)


re:				fclean $(NAME)

.PHONY:			all clean fclean re test eval

-include ${DEPS}