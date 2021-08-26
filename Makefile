NAME = webserv
SRCS = main.cpp \
		Utils/parsing.cpp \
		ConfigParsing/Config.cpp \
		Networking/Server/TestServer.cpp \
		Networking/Sockets/BindingSocket.cpp \
		Networking/Sockets/ConnectingSocket.cpp \
		Networking/Sockets/ListeningSocket.cpp \
		Networking/Sockets/SimpleSocket.cpp \
		Request/Request.cpp \
		Request/RequestMembers.cpp \
		Request/RequestUtils.cpp \
		Response/Response.cpp \
		Response/ResponseHeader.cpp \
		Cgi/CgiHandler.cpp \
		utils/utils.cpp

CXX = clang++
CXXFLAGS = -Wall -Wextra -Werror

OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY:all fclean re run
