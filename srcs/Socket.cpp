//
// Created by amyroshn on 8/16/22.
//

#include "Socket.hpp"

Socket::Socket(int port) : _socket_fd(-1), _port(port) {
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);
	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
}

Socket::Socket(const Socket *parent, int socket_fd): _socket_fd(socket_fd), _parent_socket(parent),
													_address(parent->getAddress()), _port(parent->getPort()){
}

void Socket::open() {
	int opt = 1;
	if (_socket_fd == -1) {
		if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw CannotCreateSocketException("Can't create socket");
		if (((setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR,(char *)&opt, sizeof(opt))) < 0) ||
			(setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0))
			throw CannotCreateSocketException("Can't set up socket");
		if (bind(_socket_fd, (struct sockaddr *) &_address, sizeof(_address)) == -1)
			throw CannotCreateSocketException("Can't bind socket");
		if (listen(_socket_fd, 32) == -1)
			throw CannotCreateSocketException("Can't listen socket");
	} else
		throw CannotCreateSocketException("Can't open already opened socket");
}

void Socket::close() {
	if (_socket_fd != -1) {
		::close(_socket_fd);
		_socket_fd = -1;
	}
}

Socket *Socket::accept_connection() const {
	int accepted_fd = accept(_socket_fd, NULL, NULL);
	if (accepted_fd == -1)
		throw CannotCreateSocketException("Accept error");
	fcntl(accepted_fd, F_SETFL, O_NONBLOCK);
	return new Socket(this, accepted_fd);
}

bool Socket::process_msg() {
	char *data[BUF_SIZE];
	
	ssize_t read_status = read(_socket_fd, data, BUF_SIZE);
	if (read_status == 0)
		throw CannotAccessDataException("Client closed connection");
	if (read_status == -1)
		throw CannotAccessDataException("Error of read. Connection closed");
	_client_msg.append(reinterpret_cast<const char *>(data), read_status);
	/* FIXME POTENTIAL BUF IF MSG SIZE IS GREATER THAN 65535 BITES */
	if (_client_msg.find("\r\n\r\n") != std::string::npos || _client_msg.find("\n\n") != std::string::npos) {
		Request r(_client_msg.data(), _parent_socket->getServers());
		std::cout << std::endl << std::endl << "Message from " << _socket_fd << ":" << std::endl << _client_msg << std::endl;
		_client_msg = r._rep;
		return true;
	}
	return false;
}

bool Socket::answer() {
	ssize_t write_status = write(_socket_fd, _client_msg.data(), _client_msg.size());
	if (write_status == -1)
		throw CannotAccessDataException("Can't write data in socket");
	if (write_status == static_cast<ssize_t>(_client_msg.size())) {
		std::cout << "Message to " << _socket_fd << ":" << std::endl << _client_msg << std::endl << std::endl;
		return true;
	}
	_client_msg.erase(0, static_cast<size_t>(write_status));
	std::cout << "Not all bites were sent. Message is erased and will sent another part again" << std::endl;
	return false;
}

int Socket::getSocketFd() const {
	return _socket_fd;
}

const sockaddr_in &Socket::getAddress() const {
	return _address;
}

int Socket::getPort() const {
	return _port;
}

const std::vector<const Server *> &Socket::getServers() const {
	return _servers;
}

void Socket::setServers(const Server *serv) {
	_servers.push_back(serv);
}

const Socket *Socket::getParentSocket() const {
	return _parent_socket;
}














//exception

Socket::CannotCreateSocketException::CannotCreateSocketException(const char *msg): _msg(msg) {}

const char *Socket::CannotCreateSocketException::what() const throw() {
	return _msg;
}

Socket::CannotAccessDataException::CannotAccessDataException(const char *msg): _msg(msg) {}

const char *Socket::CannotAccessDataException::what() const throw() {
	return _msg;
}
