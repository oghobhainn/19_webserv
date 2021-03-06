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

Apache et NGINX diff??rent essentiellement par la fa??on dont ils traitent les demandes Client entrantes (requ??tes)
Alors qu???Apache est bas?? sur une architecture orient??e sur les processus, la gestion des connexions NGINX repose sur un algorithme de traitement bas?? sur les ??v??nements.

L???architecture ??v??nementielle de NGINX, d???autre part, permet d???obtenir la simultan??it?? sans avoir besoin d???un processus ou d???un Thread suppl??mentaire pour chaque nouvelle connexion. 
Un seul processus NGINX peut g??rer simultan??ment des milliers de connexions HTTP

Ceci est r??alis?? par un m??canisme de boucle, appel?? Event-loop (boucle d?????v??nement). 
Ceci permet aux demandes Client d?????tre trait??es de mani??re asynchrone au sein d???un m??me thread.

Contrairement au serveur Web Apache, o?? le nombre de processus actifs ou de Threads ne peut ??tre limit?? que par des valeurs minimales et maximales, 
NGINX offre un mod??le de processus pr??visible qui est parfaitement adapt?? au mat??riel.

Tous les processus Worker d??marr??s via le processus ma??tre NGINX dans le cadre de la configuration partagent un ensemble de Listener Sockets (terminaux de communication). 
Au lieu de d??marrer un processus ou un Thread s??par?? pour chaque connexion entrante, chaque processus Worker ex??cute une boucle d?????v??nements qui permet ?? plusieurs 
milliers de connexions au sein d???un Thread d?????tre trait??es de mani??re asynchrone sans bloquer le processus

Pour ce faire, les processus Worker traitent les Listener-sockets en permanence ?? la recherche d?????v??nements d??clench??s par des connexions entrantes, 
les accepte et ex??cute les processus de lecture et d?????criture sur Socket lors du traitement HTTP.

Le traitement asynchrone des t??ches dans la boucle d?????v??nements est bas?? sur des notifications d?????v??nements, des fonctions de rappel (Callbacks) et des temporisateurs (Timers).
Ces m??canismes permettent ?? un processus Worker de d??l??guer une op??ration apr??s l???autre au syst??me d???exploitation sans attendre le r??sultat d???une op??ration ou la r??ponse des programmes Client


-----Traitement des contenus Web statiques et dynamiques: 
NGINX ne fournit que des m??canismes de diffusion de contenu Web statique. La mise ?? disposition de contenu dynamique est confi??e ?? des serveurs d'application sp??cialis??s. 
Dans ce cas, NGINX fonctionne simplement comme un proxy entre le programme client et le serveur Upsteam

-----Interpr??tation des requ??tes Client
NGINX analyse d???abord l???URL de la requ??te et l???associe aux blocs de server et location dans la configuration du serveur Web
Ce n???est qu???ensuite qu???une mise en correspondance avec le syst??me de fichiers et la combinaison avec la racine 
(qui correspond ?? la racine du document du serveur Apache) est effectu??e (si n??cessaire).
L???URL de requ??te n???est pas compar??e avec les blocs de localisation dans un bloc serveur jusqu'?? ce que le serveur demand?? soit trouv??
NGINX lit les blocs d'emplacement list??s et recherche l'emplacement qui correspond le mieux ?? la requ??te URI. 
Chaque bloc d'emplacement contient des instructions sp??cifiques qui montrent ?? NGINX comment traiter la requ??te correspondante.

https://www.youtube.com/watch?v=N49UyTlUXp4&t=19s&ab_channel=EricOMeehan