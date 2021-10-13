#include "Locations.hpp"

void add_location(t_location &my_loc, t_location &new_loc)
{
	my_loc.active = true;
	my_loc.file_extensions = new_loc.file_extensions;
	my_loc.directory = new_loc.directory;
	my_loc.http_methods = new_loc.http_methods;
	my_loc.max_body = new_loc.max_body;
	my_loc.root = new_loc.root;
	my_loc.index = new_loc.index;
	my_loc.directory_listing = new_loc.directory_listing;
	my_loc.default_file_if_request_directory = new_loc.default_file_if_request_directory;
	my_loc.file_upload_location = new_loc.file_upload_location;
}

bool files_in_dir(std::string const &path_dir, std::list<std::string> &files)
{
	struct dirent *file;
	DIR *directory;

	if ((directory = opendir(path_dir.c_str())) == 0)
		return false;
	while((file = readdir(directory)) != 0)
		files.push_back(std::string(file->d_name));
	closedir(directory);
	return true;
}

std::string dir_no_file(std::string const &path)
{
	std::string ret;
	if (path[0] == '/')
	{
		std::cout << "Error: input dir_no_file"<< std::endl;
		throw internal_server_error_exc();
	}
	if (path.find("/") == std::string::npos)
		return ".";
	return path.substr(0, path.find_last_of("/") + 1);
}

std::string found(std::string const &directory, std::string const &file)
{
	std::string ret;

	if (directory != std::string("."))
		ret = directory + file;
	else
		ret = file;
	if (is_regular_file(ret.c_str()) == 0)
		return "directory found";
	return ret;
}

std::string final_file_in_path(std::string path)
{
	if (path.find_last_of("/") == std::string::npos)
		return path;
	return path.substr(path.find_last_of("/") + 1);
}

int is_regular_file(const char *path)
{
	struct stat path_stat;
	if (stat(path, &path_stat) == -1)
	return S_ISREG(path_stat.st_mode);
	{
		std::cout << "Error: stat function" << std::endl;
		throw internal_server_error_exc();
	}
}

std::string find_file(std::string const &path_no_extension)
{
	std::string ret;
	std::string directory;
	std::list<std::string> files;
	std::string file_no_ext;

	directory = dir_no_file(path_no_extension);
	if (files_in_dir(directory, files) == false)
		return "file not found";
	file_no_ext = final_file_in_path(path_no_extension);
	for (std::list<std::string>::iterator i = files.begin(); i != files.end(); i++)
		if ((*i).substr(0, file_no_ext.size()) == file_no_ext)
			if ((*i).size() == file_no_ext.size() || (*i)[file_no_ext.size()] == '.')
			{
				if ((*i)[file_no_ext.size()] == '.')
					return found(directory, *i);
				ret = *i;
			}
	if (ret.size() != 0)
		return found(directory, ret);
	return "file not found";
}

bool accordance_method_location(std::string const &method, std::list<t_location> &loc)
{
	std::list<t_location>::iterator l = loc.begin();
	std::list<t_location>::iterator rem;
	bool found;

	if (loc.size() == 0)
		return true;
	while (l != loc.end())
	{
		found = false;
		for (std::list<std::string>::iterator met = l->http_methods.begin(); met != l->http_methods.end(); met++)
			if ((method == *met || *met == std::string("ALL")))
				found = true;
		if (found == false)
		{
			rem = l;
			l++;
			loc.erase(rem);
		}
		else
			l++;
	}
	if (loc.size() == 0)
		return false;
	return true;
}

std::string find_directory(std::string local_url, std::list<std::string> &index)
{
	std::string ret;

	if (local_url.size() != 0)
		local_url += std::string("/");
	if (index.size() != 0)
	{
		for (std::list<std::string>::iterator i = index.begin(); i != index.end(); i++)
		{
			if ((ret = find_file(local_url + *i)) != std::string("file not found") && ret != std::string("directory found"))
				return ret;
		}
	}
	return "file not found";
}

std::string find_file_directory(std::string local_root, std::string const &directory, std::list<std::string> &index, std::string method)
{
	std::string ret;
	std::string local_url;

	if (directory.find("/") != std::string::npos)
		local_url = local_root + directory.substr(directory.find_first_of("/"));
	else
		local_url = local_root;
	if (local_url[0] == '/')
		local_url = local_url.substr(1);
	if (method == std::string("PUT"))
		return local_url;
	if ((ret = find_file(local_url)) == std::string("directory found"))
		ret = find_directory(local_url, index);
	return ret;
}

std::string get_file_extension(std::string path)
{
	return path.substr(path.find_last_of('.') + 1);
}

void find_in_file_extension_location(std::list<t_location> &my_locations, t_location &loc, t_http_req &req)
{
	for (std::list<std::string>::iterator ext = loc.file_extensions.begin(); ext != loc.file_extensions.end(); ext++)
	{
		if (get_file_extension(req.URL) == *ext)
		{
			loc.FOUND_URL = req.URL;
			my_locations.push_back(loc);
		}
	}
}

void find_in_directory_location(std::list<t_location> &my_locations, t_location &loc, t_http_req &req)
{
	std::string file;
	if ((file = find_file_directory(loc.root, req.URL, loc.index, req.method)) != std::string("file not found"))
	{
		loc.FOUND_URL = file;
		my_locations.push_back(loc);
	}
}

std::string get_potential_location(std::string path)
{
	return path.substr(0, path.find_first_of("/"));
}

void prefix_location(std::list<t_location> &my_locations, t_http_req &req, t_config &conf)
{
	for (std::list<t_location>::iterator loc = conf.locations.begin(); loc != conf.locations.end(); loc++)
	{
		if (get_potential_location(req.URL) == loc->directory && loc->directory != std::string("None"))
			find_in_directory_location(my_locations, *loc, req);
	}
}

void regular_expression_location(std::list<t_location> &my_locations, t_http_req &req, t_config &conf)
{
	for (std::list<t_location>::iterator loc = conf.locations.begin(); loc != conf.locations.end(); loc++)
		if (loc->file_extensions.front() != std::string("None"))
			find_in_file_extension_location(my_locations, *loc, req);
}

void set_new_url(std::list<t_location> &my_locations, t_http_req &req)
{
	for (std::list<t_location>::iterator loc = my_locations.begin(); loc != my_locations.end(); loc++)
		if ((*loc).FOUND_URL.size() != 0 && (*loc).FOUND_URL != req.URL)
			req.URL = (*loc).FOUND_URL;
}

void URL_to_local_path(t_http_req &req, t_config &conf)
{
	std::list<t_location> my_locations;

	prefix_location(my_locations, req, conf);
	set_new_url(my_locations, req);
	regular_expression_location(my_locations, req, conf);
	if (my_locations.size() == 0)
		req.URL = "file not found";
	else if (!accordance_method_location(req.method, my_locations))
		req.URL = "method not found";
	else
	{
		add_location(req.loc, my_locations.back());
		req.URL = my_locations.back().FOUND_URL;
	}
}
