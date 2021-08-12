#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, long interface, int bklg) : BindingSocket(domain, service, protocol, port, interface)
{
    _backlog = bklg;
    start_listening();
    test_connection(_listening);
}

void ListeningSocket::start_listening()
{
    _listening = listen(get_sock(), _backlog);
}

int ListeningSocket::get_listening()
{
    return _listening;
}

int ListeningSocket::get_blacklog()
{
    return _backlog;
}