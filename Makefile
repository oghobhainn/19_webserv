NAME = webserv
SRCS = main.cpp \
		ConfigParsing/Config.cpp \
		Networking/Server/SimpleServer.cpp \
		Networking/Server/TestServer.cpp \
		Networking/Sockets/BindingSocket.cpp \
		Networking/Sockets/ConnectingSocket.cpp \
		Networking/Sockets/ListeningSocket.cpp \
		Networking/Sockets/SimpleSocket.cpp \
		request_parser/http_request_parsing.cpp\
		request_parser/utils_parser.cpp\
		response_builder/response.cpp\
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
