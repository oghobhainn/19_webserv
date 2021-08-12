#include "ConfigParsing/Config.hpp"
#include "Networking/Server/SimpleServer.hpp"
#include "Networking/Server/TestServer.hpp"
#include "Networking/Sockets/SimpleSocket.hpp"
#include "Networking/Sockets/ListeningSocket.hpp"
#include "Networking/Sockets/ConnectingSocket.hpp"
#include "Networking/Sockets/BindingSocket.hpp"

#include "main.hpp"

template<class T>
void show_list_str(T & l)
{
	std::cout << "--List string : " << std::endl;

	for (typename T::iterator it = l.begin(); it != l.end(); it++)
		std::cout << *it << std::endl;;
	std::cout << "--END" << std::endl;
}

template<class T>
void show_list(T & l)
{
	std::cout << "--List class : " << std::endl;;
	for (typename T::iterator it = l.begin(); it != l.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "--END" << std::endl;
}

std::vector<std::string> split(std::string s, const char delim) 
{
    std::vector<std::string>	result;
    std::stringstream			ss(s);
    std::string					item;

    while (getline(ss, item, delim)) 
        result.push_back(item);
    return result;
}

std::string trim(std::string str, std::string whitespace)
{
    unsigned long strBegin = str.find_first_not_of(whitespace);
    unsigned long strEnd = str.find_last_not_of(whitespace);
    unsigned long strRange = strEnd - strBegin + 1;

    if (strBegin == std::string::npos)
        return "";
    return str.substr(strBegin, strRange);
}

int countFreq(std::string pat, std::string txt)
{
    int M = pat.length();
    int N = txt.length();
    int res = 0;
   
    for (int i = 0; i <= N - M; i++)
    {
        int j;
        for (j = 0; j < M; j++)
            if (txt[i + j] != pat[j])
                break;
        if (j == M) 
        {
           res++;
           j = 0;
        }
    }
    return res;
}

int nthOccurrence(const std::string& str, const std::string& findMe, int nth)
{
    size_t  pos = 0;
    int     cnt = 0;

    while (cnt != nth)
    {
        pos += 1;
        pos = str.find(findMe, pos);
        if ( pos == std::string::npos )
            return -1;
        cnt++;
    }
    return pos;
}

std::list<class Server> parseConfig(std::string const path)
{
	std::ifstream				file(path);
    std::string					input;
    std::string					full_str;
    std::string					str_serv;
    std::string             	str_location;
    std::string             	str_without_loc;
    std::list<class Server> 	serv_list;
	std::vector<std::string>	data;
    int							nbr_serv;
    int							pos;
    int							pos_next;
    int							i = 0;

    // protect if file can't open
	if (!file.is_open())
	{
        std::cerr << "Error with the config file path\n";
		exit(1);
	}

    // create a big string
    while (std::getline(file, input))
        full_str += input;
    full_str.erase(remove_if(full_str.begin(), full_str.end(), isspace), full_str.end());

    // create a string for each server
    nbr_serv = countFreq("server", full_str);
    while (i < nbr_serv)
    {
        // create the string for each server // note: 7 = size of str server + {
        pos = nthOccurrence(full_str, "server", i);
        pos_next = nthOccurrence(full_str, "server", i + 1);
        str_serv = full_str.substr(pos + 7, pos_next - pos - 7);

        // create a class for each node and fill it with the string
        Server serv;
        serv.setFullStr(str_serv);
        serv_list.push_back(serv);

        i++;
    }

    // store in a list the locations's strings for each node and remove them for the string
    for (std::list<Server>::iterator it = serv_list.begin(); it != serv_list.end(); ++it)
    {
        i = 1;
        pos = 0;
        pos_next = 0;
        std::list<std::string> lst;
        std::string::size_type index = 0;

        // add each loc to the list
        while (pos_next != -1)
        {
            pos = nthOccurrence(it->getFullStr(), "location", i);
            pos_next = nthOccurrence(it->getFullStr(), "location", i + 1);
            str_location = it->getFullStr().substr(pos + 8, pos_next - pos - 8);
            lst.push_back(str_location);
            i++;
        }

		// create a string without the locations (host, port, etc)
        if ((index = it->getFullStr().find("location")) != std::string::npos)
            it->setStrWithoutLoc(it->getFullStr().substr(0, index));
		else
			it->setStrWithoutLoc(it->getFullStr());

        // add list to the node
        it->setLocations(lst);
    }

    // split with the ; and get the port, the host and the index
    for (std::list<Server>::iterator it2 = serv_list.begin(); it2 != serv_list.end(); ++it2)
    {
        data = split(it2->getStrWithoutLoc(), ';');
        for (std::vector<std::string>::iterator it3 = data.begin(); it3 != data.end(); ++it3)
        {
            if (it3->find("host") != std::string::npos)
                it2->setHost(it3->substr(4, it3->size() - 4));
            else if (it3->find("port") != std::string::npos)
                it2->setPort(it3->substr(4, it3->size() - 4));
        }
    }
    return serv_list;
}

int main(int argc, char **argv)
{
    std::list<class Server> serv_list;

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
        std::cout << it->getPort() << std::endl;
        std::cout << it->getHost() << std::endl;
        
		it->getLocations();
        std::cout << "-----------" << std::endl;
    }

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





