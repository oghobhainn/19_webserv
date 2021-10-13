# webserv

42 project, recoding our own web server in C++. A program capable of running an HTTP server, handling requests, and parsing configuration files.

## Resources

### Server setup

[How to build a simple HTTP server](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[Simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)

[C++ Web Programming](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)

[Transport layer, TCP client/server example. multiplexing](https://notes.shichao.io/unp/ch4/)

### HTTP 1.1 (standard to follow) :

[HTTP/1.1 (RFC 2616)](https://www.rfc-editor.org/rfc/rfc2616.html)

[HTTP/1.1 : Message Syntax and Routing (RFC 7230)](https://www.rfc-editor.org/rfc/rfc7230.html)

[HTTP/1.1 : Semantics and Content (RFC 7231)](https://www.rfc-editor.org/rfc/rfc7231.html)

[HTTP/1.1 : Conditional Requests (RFC 7232)](https://www.rfc-editor.org/rfc/rfc7232.html)

[HTTP/1.1 : Range Requests (RFC 7233)](https://www.rfc-editor.org/rfc/rfc7233.html)

[HTTP/1.1 : Caching (RFC 7234)](https://www.rfc-editor.org/rfc/rfc7234.html)

[HTTP/1.1 : Authentication (RFC 7235)](https://www.rfc-editor.org/rfc/rfc7235.html)

### Other HTTP (legacy / future) :

[HTTP/1.0 (RFC 1945)](https://www.rfc-editor.org/rfc/rfc1945.html)

[HTTP/2 (RFC 7240)](https://www.rfc-editor.org/rfc/rfc7540.html)

[HTTP/2 : Header Compression (RFC 7241)](https://www.rfc-editor.org/rfc/rfc7541.html)

[FTP (RFC 959)](https://www.rfc-editor.org/rfc/rfc959.html)

### HTTP Header Syntax

[HTTP Request Methods](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol#Request_methods)

[HTTP Status Codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)

[HTTP Header Break Style](https://stackoverflow.com/questions/5757290/http-header-line-break-style)

### Select and non-blocking

[Select](https://www.lowtek.com/sockets/select.html)

[Non-blocking I/O](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm)

### CGI

[CGI : Getting Started](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)

[CGI 1.1 Documentation](http://www.wijata.com/cgi/cgispec.html#4.0)


RFC : Request for Comments => publication from the Internet Society engineers 

https://www.ionos.fr/digitalguide/serveur/know-how/nginx-vs-apache/#:~:text=Les%20serveurs%20Web%20open%20source,traitement%20bas%C3%A9%20sur%20les%20%C3%A9v%C3%A8nements.
https://www.notion.so/Documentation-Webserv-320727979ffd4176a7dd5ba41aaadf46#fdbd4b3f5b42480d898f8838f474ef09
https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

cfr, OSI: Transport Layer of HTTP which is TCP.
NGINX is implemented on top of TCP
To implement TCP, we have to learn TCP socket programming.
A socket is the mechanism that most popular operating systems provide to give programs access to the network. It allows messages to be sent and received between applications (unrelated processes) on different networked machines.
to implement HTTP server, we have to read their particular RFC which is RFC 7230, RFC 7231, RFC 7232, RFC 7233, RFC 7234, RFC 7235.

Apache et NGINX diffèrent essentiellement par la façon dont ils traitent les demandes Client entrantes (requêtes)
Alors qu’Apache est basé sur une architecture orientée sur les processus, la gestion des connexions NGINX repose sur un algorithme de traitement basé sur les évènements.

L’architecture événementielle de NGINX, d’autre part, permet d’obtenir la simultanéité sans avoir besoin d’un processus ou d’un Thread supplémentaire pour chaque nouvelle connexion. 
Un seul processus NGINX peut gérer simultanément des milliers de connexions HTTP

Ceci est réalisé par un mécanisme de boucle, appelé Event-loop (boucle d‘événement). 
Ceci permet aux demandes Client d’être traitées de manière asynchrone au sein d’un même thread.

Contrairement au serveur Web Apache, où le nombre de processus actifs ou de Threads ne peut être limité que par des valeurs minimales et maximales, 
NGINX offre un modèle de processus prévisible qui est parfaitement adapté au matériel.

Tous les processus Worker démarrés via le processus maître NGINX dans le cadre de la configuration partagent un ensemble de Listener Sockets (terminaux de communication). 
Au lieu de démarrer un processus ou un Thread séparé pour chaque connexion entrante, chaque processus Worker exécute une boucle d’évènements qui permet à plusieurs 
milliers de connexions au sein d’un Thread d’être traitées de manière asynchrone sans bloquer le processus

Pour ce faire, les processus Worker traitent les Listener-sockets en permanence à la recherche d’événements déclenchés par des connexions entrantes, 
les accepte et exécute les processus de lecture et d’écriture sur Socket lors du traitement HTTP.

Le traitement asynchrone des tâches dans la boucle d’événements est basé sur des notifications d’événements, des fonctions de rappel (Callbacks) et des temporisateurs (Timers).
Ces mécanismes permettent à un processus Worker de déléguer une opération après l’autre au système d’exploitation sans attendre le résultat d’une opération ou la réponse des programmes Client


-----Traitement des contenus Web statiques et dynamiques: 
NGINX ne fournit que des mécanismes de diffusion de contenu Web statique. La mise à disposition de contenu dynamique est confiée à des serveurs d'application spécialisés. 
Dans ce cas, NGINX fonctionne simplement comme un proxy entre le programme client et le serveur Upsteam

-----Interprétation des requêtes Client
NGINX analyse d’abord l’URL de la requête et l’associe aux blocs de server et location dans la configuration du serveur Web
Ce n’est qu’ensuite qu’une mise en correspondance avec le système de fichiers et la combinaison avec la racine 
(qui correspond à la racine du document du serveur Apache) est effectuée (si nécessaire).
L’URL de requête n’est pas comparée avec les blocs de localisation dans un bloc serveur jusqu'à ce que le serveur demandé soit trouvé
NGINX lit les blocs d'emplacement listés et recherche l'emplacement qui correspond le mieux à la requête URI. 
Chaque bloc d'emplacement contient des instructions spécifiques qui montrent à NGINX comment traiter la requête correspondante.

https://www.youtube.com/watch?v=N49UyTlUXp4&t=19s&ab_channel=EricOMeehan