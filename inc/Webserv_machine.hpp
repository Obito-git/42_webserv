//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_WEBSERV_MACHINE_HPP
#define WEBSERV_WEBSERV_MACHINE_HPP
#include "webserv.hpp"
#include "Server.hpp"

class Webserv_machine {
private:
	std::map<int, Socket *> _machine_sockets;
	std::map<int, Socket *> _client_sockets;
	std::vector<Server *>	_servers;
	fd_set					_fd_set;
};


#endif //WEBSERV_WEBSERV_MACHINE_HPP
