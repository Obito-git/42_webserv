//
// Created by Anton on 29/08/2022.
//

#include "ListeningSocket.hpp"
#include "ClientSocket.hpp"

ListeningSocket::ListeningSocket(int port, const std::string &host) : _port(port), _host(host) {
	_socket_fd = -1;
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(host.data());
	_address.sin_port = htons(port);
	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
}

void ListeningSocket::open() {
	int opt = 1;
	std::stringstream ss;
	ss << _host << ":" << _port;
	if (_socket_fd == -1) {
		if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw CannotCreateSocketException((_log_msg = "Can't create socket for ").append(ss.str()).data());
		if (((setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR,(char *)&opt, sizeof(opt))) < 0) ||
			(setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0))
			throw CannotCreateSocketException((_log_msg = "Can't setup ").append(ss.str()).data());
		if (bind(_socket_fd, (struct sockaddr *) &_address, sizeof(_address)) < 0)
			throw CannotCreateSocketException((_log_msg = "Can't bind socket on ").append(ss.str()).data());
		if (listen(_socket_fd, 32) < 0)
			throw CannotCreateSocketException((_log_msg = "Can't listen ").append(ss.str()).data());
		Logger::println(Logger::TXT_BLACK, Logger::BG_GREEN,
						"New server socket has been created for", _host, _port);
	} else
		throw CannotCreateSocketException((_log_msg = ss.str()).append(" is already exists").data());
}

ClientSocket *ListeningSocket::accept_connection() {
	Logger::println(Logger::TXT_BLACK, Logger::BG_WHITE, "New connection on", _host, _port);
	struct sockaddr_in		addr;
	socklen_t				addr_len = sizeof(addr);
	int accepted_fd = accept(_socket_fd, (struct sockaddr *) &addr, &addr_len);
	if (accepted_fd == -1) {
		std::stringstream ss;
		ss << "Can't accept new client to " << _host << ":" << _port;
		throw CannotCreateSocketException((_log_msg = ss.str()).data());
	}
	fcntl(accepted_fd, F_SETFL, O_NONBLOCK);
	return new ClientSocket(this, accepted_fd, addr, addr_len);
}

int ListeningSocket::getPort() const {
	return _port;
}

const std::string &ListeningSocket::getHost() const {
	return _host;
}

const std::vector<const Server *> &ListeningSocket::getServers() const {
	return _servers;
}

void ListeningSocket::close() {
	if (_socket_fd != -1) {
		Logger::print(Logger::TXT_RED, Logger::BG_YELLOW, "ASocket", _host, _port);
		Logger::println(Logger::TXT_RED, Logger::BG_YELLOW, " was closed by server");
		::close(_socket_fd);
		_socket_fd = -1;
	}
}

void ListeningSocket::setServers(const Server *serv) {
	_servers.push_back(serv);
}

ListeningSocket::~ListeningSocket() {
	close();
}

std::string ListeningSocket::getAddr() const {
	return std::string(inet_ntoa(_address.sin_addr));
}
