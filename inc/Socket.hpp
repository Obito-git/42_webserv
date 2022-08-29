//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP
#include "webserv.hpp"
#include "Server.hpp"
#include "Webserv_machine.hpp"
#include "Request.hpp"

class Server;
class Webserv_machine;

class Socket {
private:
	int						_socket_fd;
	const Socket*			_parent_socket; //defined for client socket, pointed on listening socket for get _server
	struct sockaddr_in      _address;
	int						_port;
	std::string 			_host;
	std::string 			_client_msg;
	std::string				_log_msg;
	std::vector<const Server *>	_servers;
public:

/******************************************************************************************************************
 ************************************** CONSTRUCTORS/DESTRUCTORS **************************************************
 *****************************************************************************************************************/
 
	Socket(const std::string& host, int port);
	Socket(const Socket *parent, int socket_fd);

	virtual ~Socket();
	//FIXME NORMALLY NEED DESTRUCTOR WITH CLOSE INSIDE
	
/******************************************************************************************************************
 ******************************************* SOCKET METHODS *******************************************************
 *****************************************************************************************************************/
 
	void open();
	void close();
	Socket *accept_connection();
	bool process_msg(const std::map<std::string, std::string> *mime);
	bool answer();

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

	int getSocketFd() const;
	const sockaddr_in &getAddress() const;
	int getPort() const;
	const std::string &getHost() const;
	const std::vector<const Server *> &getServers() const;

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/
	
	void setServers(const Server *serv);

/******************************************************************************************************************
 *********************************************** EXCEPTIONS *******************************************************
 *****************************************************************************************************************/	

	class CannotCreateSocketException: public std::exception {
	private:
		const char *_msg;
	public:
		CannotCreateSocketException(const char *msg);
		virtual const char *what() const throw();
	};
	
	class CannotAccessDataException: public std::exception {
	private:
		const char *_msg;
	public:
		CannotAccessDataException(const char *msg);
		virtual const char *what() const throw();
	};
};


#endif //WEBSERV_SOCKET_HPP
