//
// Created by amyroshn on 8/16/22.
//

#include "Socket.hpp"

Socket::Socket(int port, Server *serv) : _port(port) {
	_servers.push_back(serv);
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
}

int Socket::getSocketFd() const {
	return _socket_fd;
}

void Socket::setSocketFd(int socketFd) {
	_socket_fd = socketFd;
}

const sockaddr_in &Socket::getAddress() const {
	return _address;
}

void Socket::setAddress(const sockaddr_in &address) {
	_address = address;
}

int Socket::getPort() const {
	return _port;
}

void Socket::setPort(int port) {
	_port = port;
}

const std::vector<Server *> &Socket::getServers() const {
	return _servers;
}

void Socket::setServers(const std::vector<Server *> &servers) {
	_servers = servers;
}

void Socket::open() {
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(_socket_fd, (struct sockaddr *)&_address, sizeof(_address));
	listen(_socket_fd, 10);
}

Socket::Socket() {}
