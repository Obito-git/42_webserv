//
// Created by Anton on 29/08/2022.
//

#ifndef WEBSERV_CLIENTSOCKET_HPP
#define WEBSERV_CLIENTSOCKET_HPP
#include "ListeningSocket.hpp"
#include "Request.hpp"


class ClientSocket: public ASocket {
public:
	ClientSocket(const ListeningSocket *parentSocket, int socket_fd, const sockaddr_in &addr, socklen_t addrLen);

private:
	const ListeningSocket	*_parent_socket; //defined for client socket, pointed on listening socket for get _server
	bool 					_close_immediately;	
	std::string 			_request;
	std::string				_response;
	struct sockaddr_in		_addr;
	socklen_t				_addr_len;
public:
	virtual void close();

	bool answer();

	bool recv_msg(const std::map<std::string, std::string> *mime);

	bool check_headers(const std::map<std::string, std::string> &headers);

	std::string getClientIp() const;
	const std::vector<const Server *> &getServers() const;

	virtual ~ClientSocket();

	std::string getClientPort() const;
};


#endif //WEBSERV_CLIENTSOCKET_HPP
