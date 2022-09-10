//
// Created by Anton on 29/08/2022.
//

#ifndef WEBSERV_CLIENTSOCKET_HPP
#define WEBSERV_CLIENTSOCKET_HPP
#include "ListeningSocket.hpp"
#include "Request.hpp"


class Request;
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

	bool dechunk(const Request &req, const std::map<std::string, std::string> *mime);
	bool check_headers(const Request &req, const std::map<std::string, std::string> *mime);
	const std::vector<const Server *> &getServers() const;

	virtual ~ClientSocket();

	std::string getClientAddr() const;
	std::string getClientPort() const;
	std::string getPort() const;
	std::string getAddr() const;
};


#endif //WEBSERV_CLIENTSOCKET_HPP
