#include "../main.hpp"

void	change_dir(std::string relative_path)
{
	char *path_name = new char[100];

	if (!(getcwd(path_name, 100)))
	{
		std::cout << "Ërror: getcwd failed" << std::endl;
		delete [] path_name;
	}
	if (chdir((std::string(path_name) + relative_path).c_str()) == -1)
	{
		std::cout << "Error: chdir failed: " << path_name + relative_path << std::endl;
		delete [] path_name;
	}
	delete [] path_name;
}
