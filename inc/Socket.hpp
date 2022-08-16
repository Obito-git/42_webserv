//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP
#include "webserv.hpp"
#include "Server.hpp"

class Socket {
private:
	int						_socket_fd;
	in_addr_t				_host;
	int						_port;
	std::vector<Server *>	_servers;
};


#endif //WEBSERV_SOCKET_HPP
