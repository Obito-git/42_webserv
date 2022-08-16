//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP
#include "Socket.hpp"
#include "webserv.hpp"
class Socket;

class Server {
private:
	std::vector<Socket *>			_sockets;
	std::set <int>					_ports;
	in_addr_t						_host;
	std::string						_server_name;
	std::map<short, std::string>	_error_pages;
	std::set<location_t>			_locations;
	location_t						_settings;
};


#endif //WEBSERV_SERVER_HPP
