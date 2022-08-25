//
// Created by amyroshn on 8/16/22.
//

#include <cstring>
#include "Webserv_machine.hpp"



void Webserv_machine::up() {
	if (_servers.empty()) {
		throw std::runtime_error("Servers empty"); //FIXME
	}
	std::map<int, Socket *>     clients_to_read;
	std::map<int, Socket *>     clients_to_write;
	run_listening_sockets();
	int max_fd_number = (--_machine_sockets.end())->first;
	
	while (42) {
		std::map<int, Socket *>::iterator it;
		fd_set read_set;
		fd_set write_set;
		int select_status = 0;
		
		
		do {
			memcpy(&read_set, &_server_fd_set, sizeof(_server_fd_set));
			FD_ZERO(&write_set);
			for (it = clients_to_write.begin(); it != clients_to_write.end(); it++)
				FD_SET(it->first, &write_set);
			std::cout << "Waiting for connections" << std::endl;
		} while (!got_signal && (select_status = select(max_fd_number + 1, &read_set, &write_set, NULL, NULL)) == 0);

		//FIXME dont need exit
		if (got_signal) {
			for (it = clients_to_write.begin(); it != clients_to_write.end(); it++) {
				it->second->close();
				delete it->second;
			}
			for (it = clients_to_read.begin(); it != clients_to_read.end(); it++) {
				it->second->close();
				delete it->second;
			}
			break;
		}
		if (select_status == -1) {
			std::cout << "Select error" << std::endl;
			exit(1);
		}

		//trying to send answer
		for (it = clients_to_write.begin(); select_status && it != clients_to_write.end(); it++) {
			if (FD_ISSET(it->first, &write_set)) {
				try {
					if (it->second->answer()) {
						clients_to_write.erase(it);
						it = clients_to_write.end();
					}
				} catch (std::exception& e){
					std::cout << e.what() << std::endl;
					it->second->close();
					FD_CLR(it->first, &_server_fd_set);
					FD_CLR(it->first, &read_set);
					FD_CLR(it->first, &write_set);
					clients_to_read.erase(it);
					clients_to_write.erase(it);
				}
				select_status = 0;
				break;
			}
		}
		
		// checking events on reading sockets
		for (it = clients_to_read.begin(); select_status && it != clients_to_read.end(); it++) {
			if (FD_ISSET(it->first, &read_set)) {
				try {
					if (it->second->process_msg(this)) {
						clients_to_write.insert(*it);		
						//FIXME need to delete from to_read?
					}
				} catch (std::exception& e) {
					std::cout << e.what() << std::endl;
					FD_CLR(it->first, &_server_fd_set);
					FD_CLR(it->first, &read_set);
					it->second->close();
					it = clients_to_read.begin();
					clients_to_read.erase(it->first);
				}
				select_status = 0;
				break;
			}
		}
		
		//checking is it new connection or not
		for (it = _machine_sockets.begin(); select_status && it != _machine_sockets.end(); it++) {
			if (FD_ISSET(it->first, &read_set)) {
				std::cout << "new connection on " << it->second->getPort() << std::endl;
				Socket *client = it->second->accept_connection(); //FIXME TRY CATCH
				clients_to_read.insert(std::make_pair(client->getSocketFd(), client));
				FD_SET(client->getSocketFd(), &_server_fd_set);
				if (client->getSocketFd() > max_fd_number)
					max_fd_number = client->getSocketFd();
				std::cout << client->getSocketFd() << " is connected" << std::endl;
				select_status = 0;
				break;
			}
		}
	}
}

void Webserv_machine::run_listening_sockets() {
	FD_ZERO(&_server_fd_set);
	for (std::vector<Server *>::iterator serv = _servers.begin(); serv != _servers.end(); serv++) {
		for (std::set<int>::iterator port = (*serv)->getPorts().begin(); port != (*serv)->getPorts().end(); port++) {
			std::map<int, Socket *>::iterator existing_socket = _machine_sockets.begin();
			for (; existing_socket != _machine_sockets.end() && (*existing_socket).second->getPort() != *port;
				existing_socket++);
			if (existing_socket == _machine_sockets.end()) {
				Socket *sock = new Socket(*port);
				sock->open(); //FIXME NEED TRY CATCH
				_machine_sockets.insert(std::make_pair(sock->getSocketFd(), sock));
				FD_SET(sock->getSocketFd(), &_server_fd_set);
			}
		}
	}

}

/******************************************************************************************************************
 ************************************** CONSTRUCTORS/DESTRUCTORS **************************************************
 *****************************************************************************************************************/


Webserv_machine::Webserv_machine(const char *path): got_signal(false) {
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

const std::vector<Server *> &Webserv_machine::getServers() const {
	return _servers;
}

const std::string &Webserv_machine::getErrorMsg() const {
	return _error_msg;
}

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/

void Webserv_machine::setMachineSockets(int port, Socket *socket) {
	_machine_sockets.insert(std::make_pair(port, socket));
	
}

void Webserv_machine::setServers(Server *server) {
	_servers.push_back(server);
}

Webserv_machine::~Webserv_machine() {
	for (std::map<int, Socket *>::iterator it = _machine_sockets.begin(); it != _machine_sockets.end(); it++) {
		it->second->close();
		delete it->second;
	}
	for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
		delete *it;
}

void Webserv_machine::setSignal(bool gotSignal) {
	got_signal = gotSignal;
}



