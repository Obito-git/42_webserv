//
// Created by amyroshn on 8/16/22.
//

#include "Server.hpp"

Server::Server() {}

const std::vector<Socket *> &Server::getSockets() const {
	return _sockets;
}

void Server::setSockets(const std::vector<Socket *> &sockets) {
	_sockets = sockets;
}

void Server::setPorts(const std::set<int> &ports) {
	_ports = ports;
}

const std::string &Server::getServerName() const {
	return _server_name;
}

void Server::setServerName(const std::string &serverName) {
	_server_name = serverName;
}

const std::map<short, std::string> &Server::getErrorPages() const {
	return _error_pages;
}

void Server::setErrorPages(const std::map<short, std::string> &errorPages) {
	_error_pages = errorPages;
}

const std::set<Location> &Server::getLocations() const {
	return _locations;
}

const Location &Server::getSettings() const {
	return _settings;
}

void Server::setSettings(const Location &settings) {
	_settings = settings;
}

const sockaddr_in &Server::getAddress() const {
	return _address;
}

void Server::setAddress(const sockaddr_in &address) {
	_address = address;
}

std::set<int> &Server::getPorts() {
	return _ports;
}

void Server::launch() {
	Socket *s = new Socket();
	s->setPort(*_ports.begin());
	s->setAddress(_address);
	_sockets.push_back(s);
	s->open();
}
