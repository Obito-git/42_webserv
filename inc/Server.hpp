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
	std::vector<Socket*>				_sockets;
	std::set <int>						_ports;
	std::string							_ip;
	struct sockaddr_in					_address;
	std::vector<std::string>			_server_name;
	std::map<std::string, Location>		_locations;
	Location							_default;
public:
	Server();
	static const std::string    _server_keywords[];
	static const std::string    _all_keywords[];
	void launch();

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

	const std::string &getIp() const;
	const std::vector<Socket *> &getSockets() const;
	const std::set<int> &getPorts() const;
	const sockaddr_in &getAddress() const;
	const std::vector<std::string> &getServerName() const;
	const std::map<std::string, Location> &getLocations() const;
	Location &getDefault();

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/
 
	void setLocations(const std::string& path, const Location &location);
	void setIp(const std::string &ip);
	void setSockets(const std::vector<Socket *> &sockets);
	void setPorts(const int& port);
	void setAddress(const sockaddr_in &address);
	void setServerName(const std::string &serverName);
	void setDefault(const Location &location);

	};


#endif //WEBSERV_SERVER_HPP
