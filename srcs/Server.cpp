//
// Created by amyroshn on 8/16/22.
//

#include "Server.hpp"
const std::string Server::_server_keywords[MAX_KEYWORDS] = {"listen", "port","server_name",
																 "error_page","client_max_body_size",
																 "file_upload","methods", "index", 
																 "autoindex", "root", "location",
																 "return", "cgi_path"};

Server::Server() {
}

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/


const std::vector<Socket *> &Server::getSockets() const {
	return _sockets;
}

const std::set<int> &Server::getPorts() const {
	return _ports;
}

const std::vector<std::string> &Server::getServerName() const {
	return _server_name;
}


const std::string &Server::getHost() const {
	return _host;
}

const std::map<std::string, Location> &Server::getLocations() const {
	return _locations;
}

Location &Server::getDefault() {
	return _default;
}

const Location &Server::getConstDefault() const{
	return _default;
}

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/


void Server::setHost(const std::string &ip) {
	_host = ip;
}

void Server::setSockets(const std::vector<Socket *> &sockets) {
	_sockets = sockets;
}

void Server::setPorts(const int& port) {
	_ports.insert(port);
}

void Server::setServerName(const std::string &serverName) {
	_server_name.push_back(serverName);
}


void Server::setLocations(const std::string& path, const Location &location) {
	_locations.erase(path);
	_locations.insert(std::make_pair(path, location));
}


void Server::setDefault(const Location &location) {
	_default = location;
}

