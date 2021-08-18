#include "ConfigParsing/Config.hpp"
#include "Networking/Server/SimpleServer.hpp"
#include "Networking/Server/TestServer.hpp"
#include "Networking/Sockets/SimpleSocket.hpp"
#include "Networking/Sockets/ListeningSocket.hpp"
#include "Networking/Sockets/ConnectingSocket.hpp"
#include "Networking/Sockets/BindingSocket.hpp"

#include "main.hpp" 

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
    std::string::size_type  index = 0;
    std::string             file_ext;
    std::string             str_location;
    Location *struct_loc = NULL;
    int found = 0;
    int pos;
    int	pos_next;
    int beg = 0;
    int end = 0;
    int i = 1;
    int j = 0;

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
        }
    }
    return serv_list;
}

int main(int argc, char **argv)
{
    std::list<Server> serv_list;

    if (argc != 2)
    {
        std::cout << "ERROR: Wrong Arguments" << std::endl;
        return 1;
    }
    serv_list = parseConfig(argv[1]);
    
    ///////////////////// Print results /////////////////////////
    std::string test2;

    for (std::list<class Server>::iterator it = serv_list.begin(); it != serv_list.end(); ++it)
    {
        std::cout << "---------------------- BEGIN ----------------------------" << std::endl;
        std::cout << "Port: " << it->getPort() << std::endl;
        std::cout << "Host: "<< it->getHost() << std::endl;
        std::cout << "Root: "<< it->getRoot() << std::endl;
        std::cout << "Ser Name: "<< it->getServerName() << std::endl;
        
		it->getLocations();
        std::cout << "---------------------- END --------------------------------" << std::endl;
    }

    //// TO DO
    
    // - default error pages
    // - limit client body size
    // - in the roots : 
    //      - one HTTP redirection
    //      - define a directory or a file from where the file should be search (for example
    //        if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is
    //        /tmp/www/pouic/toto/pouet)
    //      - turn on or off directory listing
    //      - default file to answer if the request is a directory
    //      - CGI
    //      - make the route able to accept uploaded files and configure where it should be saved




    ////////////////////// Server ////////////////////////////////
    TestServer t(80);
    return 0;
}









////////////////////////////////////////////////////////////////////////////

// RFC : Request for Comments => publication from the Internet Society engineers 

// https://www.ionos.fr/digitalguide/serveur/know-how/nginx-vs-apache/#:~:text=Les%20serveurs%20Web%20open%20source,traitement%20bas%C3%A9%20sur%20les%20%C3%A9v%C3%A8nements.
// https://www.notion.so/Documentation-Webserv-320727979ffd4176a7dd5ba41aaadf46#fdbd4b3f5b42480d898f8838f474ef09
// https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

// cfr, OSI: Transport Layer of HTTP which is TCP.
// NGINX is implemented on top of TCP
// To implement TCP, we have to learn TCP socket programming.
// A socket is the mechanism that most popular operating systems provide to give programs access to the network. It allows messages to be sent and received between applications (unrelated processes) on different networked machines.
// to implement HTTP server, we have to read their particular RFC which is RFC 7230, RFC 7231, RFC 7232, RFC 7233, RFC 7234, RFC 7235.

// Apache et NGINX diffèrent essentiellement par la façon dont ils traitent les demandes Client entrantes (requêtes)
// Alors qu’Apache est basé sur une architecture orientée sur les processus, la gestion des connexions NGINX repose sur un algorithme de traitement basé sur les évènements.

//L’architecture événementielle de NGINX, d’autre part, permet d’obtenir la simultanéité sans avoir besoin d’un processus ou d’un Thread supplémentaire pour chaque nouvelle connexion. 
//Un seul processus NGINX peut gérer simultanément des milliers de connexions HTTP

// Ceci est réalisé par un mécanisme de boucle, appelé Event-loop (boucle d‘événement). 
//Ceci permet aux demandes Client d’être traitées de manière asynchrone au sein d’un même thread.

// Contrairement au serveur Web Apache, où le nombre de processus actifs ou de Threads ne peut être limité que par des valeurs minimales et maximales, 
// NGINX offre un modèle de processus prévisible qui est parfaitement adapté au matériel.

// Tous les processus Worker démarrés via le processus maître NGINX dans le cadre de la configuration partagent un ensemble de Listener Sockets (terminaux de communication). 
// Au lieu de démarrer un processus ou un Thread séparé pour chaque connexion entrante, chaque processus Worker exécute une boucle d’évènements qui permet à plusieurs 
// milliers de connexions au sein d’un Thread d’être traitées de manière asynchrone sans bloquer le processus

// Pour ce faire, les processus Worker traitent les Listener-sockets en permanence à la recherche d’événements déclenchés par des connexions entrantes, 
// les accepte et exécute les processus de lecture et d’écriture sur Socket lors du traitement HTTP.

// Le traitement asynchrone des tâches dans la boucle d’événements est basé sur des notifications d’événements, des fonctions de rappel (Callbacks) et des temporisateurs (Timers).
// Ces mécanismes permettent à un processus Worker de déléguer une opération après l’autre au système d’exploitation sans attendre le résultat d’une opération ou la réponse des programmes Client


// -----Traitement des contenus Web statiques et dynamiques: 
// NGINX ne fournit que des mécanismes de diffusion de contenu Web statique. La mise à disposition de contenu dynamique est confiée à des serveurs d'application spécialisés. 
// Dans ce cas, NGINX fonctionne simplement comme un proxy entre le programme client et le serveur Upsteam

// -----Interprétation des requêtes Client
// NGINX analyse d’abord l’URL de la requête et l’associe aux blocs de server et location dans la configuration du serveur Web
// Ce n’est qu’ensuite qu’une mise en correspondance avec le système de fichiers et la combinaison avec la racine 
// (qui correspond à la racine du document du serveur Apache) est effectuée (si nécessaire).
// L’URL de requête n’est pas comparée avec les blocs de localisation dans un bloc serveur jusqu'à ce que le serveur demandé soit trouvé
// NGINX lit les blocs d'emplacement listés et recherche l'emplacement qui correspond le mieux à la requête URI. 
// Chaque bloc d'emplacement contient des instructions spécifiques qui montrent à NGINX comment traiter la requête correspondante.


// -----Configuration
// NGINX est centralise sur un seul fichier .conf

/////////////////////////////////////////////////////////////////////////////////////

// https://www.youtube.com/watch?v=N49UyTlUXp4&t=19s&ab_channel=EricOMeehan





