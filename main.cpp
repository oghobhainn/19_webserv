#include "main.hpp"
#include "ConfigParsing/Config.hpp"
#include "Server/Cluster.hpp"
#include "Parsing/Parsing.hpp"

int main(int argc, char **argv)
{
    std::list<Server> serv_list;
    std::vector<int> set_of_port;

    if (argc != 2)
    {
        std::cout << "ERROR: Wrong Arguments" << std::endl;
        return 1;
    }
    serv_list = parseConfig(argv[1]);
    Cluster	cluster(serv_list);

    if (cluster.setup() == -1)
		return (1);
	cluster.run();
    cluster.clean();
    return 0;
}