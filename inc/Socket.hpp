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
	//Socket*					_parent_socket; //FIXME
	struct sockaddr_in      _address;
	int						_port;
	std::string 			_client_msg;
	std::vector<Server *>	_servers;
public:
	Socket(int port);
	Socket(int socket_fd, int port);
	
	void open();
	void close();
	Socket *accept_connection() const;
	bool process_msg(Webserv_machine *ws);
	bool answer();

	int getSocketFd() const;
	const sockaddr_in &getAddress() const;
	int getPort() const;
	

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
