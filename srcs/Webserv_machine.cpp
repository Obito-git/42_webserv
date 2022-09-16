//
// Created by amyroshn on 8/16/22.
//

#include <cstring>
#include "Webserv_machine.hpp"



void Webserv_machine::up() {
	if (_servers.empty()) {
		return;
	}
	std::map<int, ClientSocket *>     clients_to_read;
	std::map<int, ClientSocket *>     clients_to_write;
	if (!run_listening_sockets())
		return ;
	int max_fd_number = (--_machine_sockets.end())->first;
	
	Logger::println("Waiting for connections...");
	while (42) {
		std::map<int, ClientSocket *>::iterator it;
		fd_set read_set;
		fd_set write_set;
		int select_status = 0;
		
		
		do {
			FD_ZERO(&read_set);
			memcpy(&read_set, &_server_fd_set, sizeof(_server_fd_set));
			FD_ZERO(&write_set);
			for (it = clients_to_write.begin(); it != clients_to_write.end(); it++)
				FD_SET(it->first, &write_set);
		} while (!got_shutdown_signal && (select_status = select(max_fd_number + 1, &read_set, &write_set, NULL, NULL)) == 0);
		if (got_shutdown_signal || select_status <= 0) {
			for (it = clients_to_write.begin(); it != clients_to_write.end(); it++) {
				it->second->close();
				delete it->second;
			}
			for (it = clients_to_read.begin(); it != clients_to_read.end(); it++) {
				it->second->close();
				delete it->second;
			}
			clients_to_write.clear();
			clients_to_read.clear();
			if (got_shutdown_signal) {
				Logger::println(Logger::TXT_BLACK, Logger::BG_RED, "Got shutdown signal");
				break;
			}
			FD_ZERO(&_server_fd_set);
			for (std::map<int, ListeningSocket *>::iterator y = _machine_sockets.begin(); y != _machine_sockets.end(); y++)
				FD_SET(y->first, &_server_fd_set);
			Logger::println(Logger::TXT_BLACK, Logger::BG_RED, "Select returned 0");
			Logger::println("Waiting for connections...");
			continue;
		}

		//trying to send answer
		for (it = clients_to_write.begin(); select_status && it != clients_to_write.end(); it++) {
			if (FD_ISSET(it->first, &write_set)) {
				try {
					if (it->second->answer()) {
						clients_to_write.erase(it->first);
					}
				} catch (std::exception& e){
					Logger::println(Logger::TXT_BLACK,Logger::BG_RED, e.what());
					FD_CLR(it->first, &_server_fd_set);
					delete it->second;
					clients_to_read.erase(it->first);
					clients_to_write.erase(it->first);
				}
				select_status = 0;
				break;
			}
		}
		
		// checking events on reading sockets
		for (it = clients_to_read.begin(); select_status && it != clients_to_read.end(); it++) {
			if (FD_ISSET(it->first, &read_set)) {
				try {
					if (it->second->recv_msg(_mime))
						clients_to_write.insert(*it);		
				} catch (std::exception& e) {
					FD_CLR(it->first, &_server_fd_set);
					delete it->second;
					clients_to_read.erase(it->first);
				}
				select_status = 0;
				break;
			}
		}
		
		//checking is it new connection or not
		for (std::map<int, ListeningSocket *>::iterator y = _machine_sockets.begin();
				select_status && y != _machine_sockets.end(); y++) {
			if (FD_ISSET(y->first, &read_set)) {
				ClientSocket *client;
				try {
					client = y->second->accept_connection();
				} catch (std::exception& e) {
					Logger::println(Logger::TXT_BLACK,Logger::BG_RED, e.what());
					break;
				}
				clients_to_read.insert(std::make_pair(client->getSocketFd(), client));
				FD_SET(client->getSocketFd(), &_server_fd_set);
				if (client->getSocketFd() > max_fd_number)
					max_fd_number = client->getSocketFd();
				break;
			}
		}
	}
}

bool Webserv_machine::run_listening_sockets() {
	FD_ZERO(&_server_fd_set);
	for (std::vector<Server *>::iterator serv = _servers.begin(); serv != _servers.end(); serv++) {
		for (std::set<int>::iterator port = (*serv)->getPorts().begin(); port != (*serv)->getPorts().end(); port++) {
			std::map<int, ListeningSocket *>::iterator existing_socket = _machine_sockets.begin();
			while (existing_socket != _machine_sockets.end()) {
				if ((*existing_socket).second->getPort() == *port && 
				(*serv)->getHost() == (*existing_socket).second->getHost())
					break;
				existing_socket++;
			}
			if (existing_socket == _machine_sockets.end()) {
				ListeningSocket *sock = new ListeningSocket(*port, (*serv)->getHost());
				try {
					sock->open();
				} catch (std::exception& e) {
					Logger::println(Logger::BG_RED, e.what());
					delete sock;
					return false;
				}
				_machine_sockets.insert(std::make_pair(sock->getSocketFd(), sock));
				FD_SET(sock->getSocketFd(), &_server_fd_set);
				sock->setServers(*serv);
			} else
				existing_socket->second->setServers(*serv);
		}
	}
	return true;
}

/******************************************************************************************************************
 ************************************** CONSTRUCTORS/DESTRUCTORS **************************************************
 *****************************************************************************************************************/


Webserv_machine::Webserv_machine(const char *path, const std::string &webserv_path): _mime(NULL), got_shutdown_signal(false) {
	ConfigParser config(path, webserv_path);
	std::string error_msg;
	try {
		_servers = config.getParsedServers();
		_mime = config.getMime();
		return;
	} catch (ConfigParser::ConfigNoRequiredKeywords& e) {
		error_msg = e.what();
		size_t delim_pos = error_msg.find(':');
		Logger::println(Logger::TXT_YELLOW, Logger::BG_RED, error_msg.substr(0, delim_pos + 1));
		Logger::println(error_msg.substr(delim_pos + 2));
	} catch (std::exception& e) {
			error_msg = e.what();
			size_t delim_pos = error_msg.find(':');
			if (delim_pos == std::string::npos)
				Logger::println(Logger::TXT_YELLOW, Logger::BG_RED, error_msg);
			else {
				Logger::println(Logger::TXT_YELLOW, Logger::BG_RED, error_msg.substr(0, delim_pos + 1));
				if (error_msg.substr(delim_pos + 2).empty())
					Logger::println(Logger::BG_YELLOW, "\t\t\t\t\t\t\t");
				else
					Logger::println(Logger::TXT_BLACK, Logger::BG_YELLOW, error_msg.substr(delim_pos + 2));
			}
		}
	std::vector<Server *>& s = config.getServers();
	for (std::vector<Server *>::iterator it = s.begin(); it != s.end(); it++)
		delete *it;
	_servers.clear();
}

Webserv_machine::~Webserv_machine() {
	for (std::map<int, ListeningSocket *>::iterator it = _machine_sockets.begin(); it != _machine_sockets.end(); it++) {
		it->second->close();
		delete it->second;
	}
	for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
		delete *it;
	delete _mime;
}

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/


/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/

void Webserv_machine::setServers(Server *server) {
	_servers.push_back(server);
}


void Webserv_machine::setSignal(bool gotSignal) {
	got_shutdown_signal = gotSignal;
}



