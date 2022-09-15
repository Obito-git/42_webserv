//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_WEBSERV_MACHINE_HPP
#define WEBSERV_WEBSERV_MACHINE_HPP
#include "webserv.hpp"
#include "ConfigParser.hpp"
#include "ClientSocket.hpp"

class ListeningSocket;
class ClientSocket;

class Webserv_machine {
private:
	std::map<int, ListeningSocket *>     _machine_sockets; // <socket_fd, socket>
	std::vector<Server *>	    _servers;
	fd_set				    	_server_fd_set;
	std::map<std::string, std::string> *_mime;
	bool 						got_shutdown_signal;

	bool run_listening_sockets();
public:
	/*      construct / destruct        */
	Webserv_machine(const char *path, const std::string &webserv_path);

	virtual ~Webserv_machine();

	void up();

	void setSignal(bool gotSignal);

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

	
/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/

	void setServers(Server *server);

};


#endif //WEBSERV_WEBSERV_MACHINE_HPP
