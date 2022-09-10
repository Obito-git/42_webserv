//
// Created by amyroshn on 8/16/22.
//

#include "Server.hpp"
const std::string Server::_server_keywords[MAX_KEYWORDS] = {"listen", "port","server_name",
																 "error_page","client_max_body_size",
																 "file_upload","methods", "index", 
																 "autoindex", "root", "location",
																 "return", "cgi_path"};

const std::string Server::_cgi_types[MAX_CGI_TYPES] = {"php", "py", "bla"};

Server::Server() {
}

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

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

const Location &Server::getConstDefault() const{
	return _default;
}

const std::map<std::string, std::string> &Server::getCgiPaths() const {
	return _cgi_paths;
}

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/


void Server::setHost(const std::string &ip) {
	_host = ip;
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

void Server::setCgiPaths(const std::string& type, const std::string& path) {
	_cgi_paths.erase(type);
	_cgi_paths.insert(std::make_pair(type, path));
}

void Server::setAllLocations(const std::map<std::string, Location> &locations) {
	_locations = locations;
}
