#include "Parsing.hpp"

std::list<class Server> get_serv_list(std::string full_str)
{
    std::string					str_serv;
    std::list<class Server> 	serv_list;
    int							nbr_serv;
    int							i = 0;
    int							pos;
    int							pos_next;
    Server *serv = NULL;

    nbr_serv = countFreq("server", full_str);
    while (i < nbr_serv)
    {
        serv = new Server;
        pos = nthOccurrence(full_str, "server", i);
        pos_next = nthOccurrence(full_str, "server", i + 1);
        str_serv = full_str.substr(pos + 7, pos_next - pos - 7);
        serv->setFullStr(str_serv);
        serv->setServerName(i);
        serv_list.push_back(*serv);
        i++;
    }
    return serv_list;
}

void parse_loc(std::list<class Server> &serv_list)
{
    std::string             str_methods;
    std::string             file_ext;
    std::string             str_location;
    Location *struct_loc = NULL;
    std::string::size_type  index = 0;
    int i = 1;
    int j = 0;
    int pos;
    int	pos_next;
    int beg = 0;
    int end = 0;
    int found = 0;

    for (std::list<Server>::iterator it = serv_list.begin(); it != serv_list.end(); ++it)
    {
        i = 1;
        j = 0;
        pos = 0;
        pos_next = 0;
        found = 0;
        beg = 0;
        end = 0;
        index = 0;

        it->setNbLoc(countFreq("location", it->getFullStr()));
        it->locations = new Location[it->getNbLoc()];
        while (pos_next != -1)
        {
            struct_loc = new Location;
            pos = nthOccurrence(it->getFullStr(), "location", i);
            pos_next = nthOccurrence(it->getFullStr(), "location", i + 1);
            str_location = it->getFullStr().substr(pos + 8, pos_next - pos - 8);
            it->locations[j].full_str = str_location; 
            found = str_location.find("{");
            file_ext = str_location.substr(0, found);
            it->locations[j].extension = file_ext;
            it->locations[j].file_extensions = split_lst(file_ext, '/');
            if (str_location.find("root") != std::string::npos)
            {
                beg = str_location.find("root");
                end = str_location.find(";", beg);
                it->locations[j].root = str_location.substr(beg + 4, len(str_location) - beg - 4 - (len(str_location) - end));
            }
            if (str_location.find("http_methods") != std::string::npos)
            {
                beg = str_location.find("http_methods");
                end = str_location.find(";", beg);
                str_methods = str_location.substr(beg + 12, len(str_location) - beg - 12 - (len(str_location) - end));
                if (str_methods.find("GET") != std::string::npos)
                    it->locations[j].get_method = true;
                if (str_methods.find("POST") != std::string::npos)
                    it->locations[j].post_method = true;
                if (str_methods.find("DELETE") != std::string::npos)
                    it->locations[j].delete_method = true;
            }
            if (str_location.find("index") != std::string::npos)
            {
                beg = str_location.find("index");
                end = str_location.find(";", beg);
                it->locations[j].index = str_location.substr(beg + 5, len(str_location) - beg - 5 - (len(str_location) - end));
            }
            if (str_location.find("redirection") != std::string::npos)
            {
                beg = str_location.find("redirection");
                end = str_location.find(";", beg);
                it->locations[j].redirection = str_location.substr(beg + 11, len(str_location) - beg - 11 - (len(str_location) - end));
            }
            if (str_location.find("file_upload_location") != std::string::npos)
            {
                beg = str_location.find("file_upload_location");
                end = str_location.find(";", beg);
                it->locations[j].file_upload_location = str_location.substr(beg + 20, len(str_location) - beg - 20 - (len(str_location) - end));
            }
            if (str_location.find("default_file") != std::string::npos)
            {
                beg = str_location.find("default_file");
                end = str_location.find(";", beg);
                it->locations[j].default_file_if_request_directory = str_location.substr(beg + 12, len(str_location) - beg - 12 - (len(str_location) - end));
            }
            if (str_location.find("autoindex") != std::string::npos)
            {
                beg = str_location.find("autoindex");
                end = str_location.find(";", beg);
                if (str_location.substr(beg + 9, len(str_location) - beg - 9 - (len(str_location) - end)).find("off") != std::string::npos)
                    it->locations[j].autoindex = false;
            }
            i++;
            j++;
        }
        if ((index = it->getFullStr().find("location")) != std::string::npos)
            it->setStrWithoutLoc(it->getFullStr().substr(0, index));
		else
			it->setStrWithoutLoc(it->getFullStr());
    }
}

std::list<class Server> parseConfig(std::string const path)
{
	std::ifstream				file(path);
    std::string					input;
    std::string					full_str;
    std::string             	str_location;
    std::string             	str_without_loc;
    std::list<class Server> 	serv_list;
	std::vector<std::string>	data;

	if (!file.is_open())
	{
        std::cerr << "Error with the config file path\n";
		exit(1);
	}
    while (std::getline(file, input))
        full_str += input;
    full_str.erase(remove_if(full_str.begin(), full_str.end(), isspace), full_str.end());
    serv_list = get_serv_list(full_str);
    parse_loc(serv_list);
    for (std::list<Server>::iterator it2 = serv_list.begin(); it2 != serv_list.end(); ++it2)
    {
        data = split(it2->getStrWithoutLoc(), ';');
        for (std::vector<std::string>::iterator it3 = data.begin(); it3 != data.end(); ++it3)
        {
            if (it3->find("host") != std::string::npos)
                it2->setHost(it3->substr(4, it3->size() - 4));
            else if (it3->find("port") != std::string::npos)
                it2->setPort(it3->substr(4, it3->size() - 4));
            else if (it3->find("root") != std::string::npos)
                it2->setRoot(it3->substr(4, it3->size() - 4));
            else if (it3->find("default_error_page") != std::string::npos)
                it2->setDefaultErrorPage(it3->substr(18, it3->size() - 18));
            else if (it3->find("client_body_size") != std::string::npos)
                it2->setClientBodySize(it3->substr(16, it3->size() - 16));
            else if (it3->find("autoindex") != std::string::npos)
                it2->setAutoIndex(it3->substr(9, it3->size() - 9));
            else if (it3->find("index") != std::string::npos)
                it2->setIndex(it3->substr(5, it3->size() - 5));
            else if (it3->find("http_methods") != std::string::npos)
            {
                if (it3->find("GET") != std::string::npos)
                    it2->setGetMethod(true);
                if (it3->find("POST") != std::string::npos)
                    it2->setPostMethod(true);
                if (it3->find("DELETE") != std::string::npos)
                    it2->setDeleteMethod(true);
            }
        }
    }
    return serv_list;
}