//
// Created by Anton on 29/08/2022.
//

#ifndef WEBSERV_LISTENINGSOCKET_HPP
#define WEBSERV_LISTENINGSOCKET_HPP
#include "ASocket.hpp"
#include "Server.hpp"

class ClientSocket;
class ListeningSocket: public ASocket {
private:
	struct sockaddr_in			_address;
	int							_port;
	std::string 				_host;
	std::vector<const Server *>	_servers;
	
public:
	ListeningSocket(int port, const std::string &host);
	void open();

	virtual void close();

	int getPort() const;

	const std::string &getHost() const;
	
	std::string getAddr() const;

	const std::vector<const Server *> &getServers() const;

	ClientSocket *accept_connection();

	void setServers(const Server *serv);

	virtual ~ListeningSocket();
};


#endif //WEBSERV_LISTENINGSOCKET_HPP
