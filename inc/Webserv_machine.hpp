//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_WEBSERV_MACHINE_HPP
#define WEBSERV_WEBSERV_MACHINE_HPP
#include "webserv.hpp"
#include "Server.hpp"
#include "ConfigParser.hpp"

class Webserv_machine {
private:
	Webserv_machine();

	std::map<int, Socket *>     _machine_sockets;
	std::map<int, Socket *>     _client_sockets;
	std::vector<Server *>	    _servers;
	std::string 				_error_msg;
	fd_set				    	_fd_set;

public:
	/*      construct / destruct        */
	Webserv_machine(const char *path);
	
	void up();

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

	const std::map<int, Socket *> &getMachineSockets() const;
	const std::map<int, Socket *> &getClientSockets() const;
	const std::vector<Server *> &getServers() const;
	const std::string &getErrorMsg() const;
	const fd_set &getFdSet() const;
	
/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/

	void setMachineSockets(int fd, Socket *socket);
	void setClientSockets(int fd, Socket *socket);
	void setServers(Server *server);
	void setErrorMsg(const std::string &errorMsg);
	void setFdSet(const fd_set &fdSet);

};


#endif //WEBSERV_WEBSERV_MACHINE_HPP
