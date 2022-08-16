//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP
#include "webserv.hpp"
#include "Server.hpp"

class Server;

class Socket {
private:
	int						_socket_fd;
	struct sockaddr_in      _address;
	//in_addr_t				_host;
	int						_port;
	std::vector<Server *>	_servers;
public:
	void open();

	Socket();

	int getSocketFd() const;

	void setSocketFd(int socketFd);

	const sockaddr_in &getAddress() const;

	void setAddress(const sockaddr_in &address);

	int getPort() const;

	void setPort(int port);

	const std::vector<Server *> &getServers() const;

	void setServers(const std::vector<Server *> &servers);

public:
	Socket(int port, Server* serv);

	class CannotCreateSocketException: public std::exception {
	public:
		virtual const char *what() const throw();
	};
};


#endif //WEBSERV_SOCKET_HPP
