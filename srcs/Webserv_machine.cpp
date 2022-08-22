//
// Created by amyroshn on 8/16/22.
//

#include "Webserv_machine.hpp"



void Webserv_machine::up() {
	if (_servers.empty()) {
		throw std::runtime_error("Servers empty"); //FIXME
	}
	Server *serv = *_servers.begin();
	
	/* FIX THIS PART */
	struct sockaddr_in			addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(*serv->getPorts().begin());
	memset(addr.sin_zero, '\0', sizeof addr.sin_zero);
	/************************/

	std::string html_page;
	try {
		html_page = ft_read_file("site/index.html");
	} catch (std::exception& e) {
		std::cout << "can't open index html" << std::endl;
	}
	/***************** TEST ***********************/
	std::string hello_str = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	hello_str.append(std::to_string(html_page.size()).append("\n\n")).append(html_page);
	const char* hello = hello_str.data();
	/**********************************************/
	serv->setAddress(addr);
	serv->launch();
	while (true) {
		int new_socket;
		int valread;
		int addr_len = sizeof(serv->getAddress());
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((new_socket = accept((*serv->getSockets().begin())->getSocketFd(),
								 (struct sockaddr *)&serv->getAddress(),
										 (socklen_t*)&addr_len))<0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}

		char buffer[30000] = {0};
		valread = read( new_socket , buffer, 30000);
		printf("%s\n",buffer );
		write(new_socket , hello , strlen(hello));
		printf("------------------Hello message sent-------------------");
		close(new_socket);
	}
}

/******************************************************************************************************************
 ************************************** CONSTRUCTORS/DESTRUCTORS **************************************************
 *****************************************************************************************************************/


Webserv_machine::Webserv_machine(const char *path) {
	ConfigParser config(path);
	try {
		_servers = config.getServers();
	} catch (std::exception &e) {
		_error_msg =  e.what();
		return;
	}
	_error_msg = "OK";
}

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

const std::map<int, Socket *> &Webserv_machine::getMachineSockets() const {
	return _machine_sockets;
}

const std::map<int, Socket *> &Webserv_machine::getClientSockets() const {
	return _client_sockets;
}

const std::vector<Server *> &Webserv_machine::getServers() const {
	return _servers;
}

const std::string &Webserv_machine::getErrorMsg() const {
	return _error_msg;
}

const fd_set &Webserv_machine::getFdSet() const {
	return _fd_set;
}

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/

void Webserv_machine::setMachineSockets(int fd, Socket *socket) {
	_machine_sockets.insert(std::make_pair(fd, socket));
	
}
void Webserv_machine::setClientSockets(int fd, Socket *socket) {
	_client_sockets.insert(std::make_pair(fd, socket));	
}

void Webserv_machine::setServers(Server *server) {
	_servers.push_back(server);
}

void Webserv_machine::setErrorMsg(const std::string &errorMsg) {
	_error_msg = errorMsg;
}

void Webserv_machine::setFdSet(const fd_set &fdSet) {
	_fd_set = fdSet;
}

