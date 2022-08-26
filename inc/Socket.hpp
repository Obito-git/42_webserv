//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP
#include "webserv.hpp"
#include "Server.hpp"
#include "Webserv_machine.hpp"
#include "Request.hpp"

class Server;
class Webserv_machine;

class Socket {
private:
	int						_socket_fd;
	const Socket*			_parent_socket; //defined for client socket, pointed on listening socket for get _server
	struct sockaddr_in      _address;
	int						_port;
	std::string 			_client_msg;
	std::vector<const Server *>	_servers;
public:
	Socket(int port);
	Socket(const Socket *parent, int socket_fd);
	
	void open();
	void close();
	Socket *accept_connection() const;
	bool process_msg();
	bool answer();

	int getSocketFd() const;
	const sockaddr_in &getAddress() const;
	int getPort() const;

	std::vector<const Server *> getServers() const;
	void setServers(const Server *serv);

	const Socket *getParentSocket() const;


	class CannotCreateSocketException: public std::exception {
	private:
		const char *_msg;
	public:
		CannotCreateSocketException(const char *msg);
		virtual const char *what() const throw();
	};
	
	class CannotAccessDataException: public std::exception {
	private:
		const char *_msg;
	public:
		CannotAccessDataException(const char *msg);
		virtual const char *what() const throw();
	};
};


#endif //WEBSERV_SOCKET_HPP
