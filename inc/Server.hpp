//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP
#include "Socket.hpp"
#include "webserv.hpp"
#include "Location.hpp"

class Socket;

class Server {
private:
	std::vector<Socket*>			_sockets;
	std::set <int>					_ports;
	//in_addr_t						_host;
	struct sockaddr_in              _address;
	std::string						_server_name;
	std::map<short, std::string>	_error_pages;
	std::set<Location>		        _locations;
	Location settings;
public:
	const sockaddr_in &getAddress() const;
	void setAddress(const sockaddr_in &address);
	void launch();

private:
	Location						_settings;

public:
	Server();

	const std::vector<Socket *> &getSockets() const;

	void setSockets(const std::vector<Socket *> &sockets);

	std::set<int> &getPorts() ;

	void setPorts(const std::set<int> &ports);

	const std::string &getServerName() const;

	void setServerName(const std::string &serverName);

	const std::map<short, std::string> &getErrorPages() const;

	void setErrorPages(const std::map<short, std::string> &errorPages);

	const std::set<Location> &getLocations() const;

	void setLocations(const std::set<Location> &locations);

	const Location &getSettings() const;

	void setSettings(const Location &settings);
};


#endif //WEBSERV_SERVER_HPP
