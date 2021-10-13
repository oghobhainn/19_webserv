NAME = webserv
SRCS = main.cpp \
		Utils/parsing.cpp \
		Parsing/Parsing.cpp \
		ConfigParsing/Config.cpp \
		Request/Request.cpp \
		Request/RequestMembers.cpp \
		Request/RequestUtils.cpp \
		Response/Response.cpp \
		Response/ResponseHeader.cpp \
		utils/utils.cpp \
		Server/Cluster.cpp \
		Server/ActiveServer.cpp \

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
