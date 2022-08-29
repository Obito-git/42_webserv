//
// Created by amyroshn on 8/16/22.
//

#include "Socket.hpp"

/******************************************************************************************************************
 ************************************** CONSTRUCTORS/DESTRUCTORS **************************************************
 *****************************************************************************************************************/
 
Socket::Socket(const std::string& host, int port) : _socket_fd(-1), _parent_socket(NULL), _port(port), _host(host) {
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(host.data());
	_address.sin_port = htons(port);
	memset(_address.sin_zero, '\0', sizeof _address.sin_zero);
}

Socket::Socket(const Socket *parent, int socket_fd): _socket_fd(socket_fd), _parent_socket(parent),
													_address(parent->getAddress()), _port(parent->getPort()),
													_host(parent->getHost()) {
	std::stringstream ss;
	ss << "Client " << _socket_fd << " has been connected to " << _host << ":" << _port;
	Logger::println(Logger::TXT_BLACK, Logger::BG_WHITE, ss.str());
}

Socket::~Socket() {
	close();
}
/******************************************************************************************************************
 ******************************************* SOCKET METHODS *******************************************************
 *****************************************************************************************************************/
 
void Socket::open() {
	int opt = 1;
	std::stringstream ss;
	ss << _host << ":" << _port;
	if (_socket_fd == -1) {
		if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw CannotCreateSocketException((_log_msg = "Can't create socket for ").append(ss.str()).data());
		if (((setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR,(char *)&opt, sizeof(opt))) < 0) ||
			(setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0))
			throw CannotCreateSocketException((_log_msg = "Can't setup ").append(ss.str()).data());
		if (bind(_socket_fd, (struct sockaddr *) &_address, sizeof(_address)) < 0)
			throw CannotCreateSocketException((_log_msg = "Can't bind socket on ").append(ss.str()).data());
		if (listen(_socket_fd, 32) < 0)
			throw CannotCreateSocketException((_log_msg = "Can't listen ").append(ss.str()).data());
		Logger::println(Logger::TXT_BLACK, Logger::BG_GREEN,
						"New server socket has been created for", _host, _port);
	} else
		throw CannotCreateSocketException((_log_msg = ss.str()).append(" is already exists").data());
}

void Socket::close() {
	if (_socket_fd != -1) {
		if (_parent_socket) {
			Logger::print(Logger::TXT_BLACK, Logger::BG_WHITE, "Client", _socket_fd, "has been disconnected from ");
			Logger::println(Logger::TXT_BLACK, Logger::BG_WHITE, _host ,_port);
		} else {
			Logger::print(Logger::TXT_RED, Logger::BG_YELLOW, "Socket", _host, _port);
			Logger::println(Logger::TXT_RED, Logger::BG_YELLOW, " was closed by server");
		}
		::close(_socket_fd);
		_socket_fd = -1;
	}
}

Socket *Socket::accept_connection() {
	Logger::println(Logger::TXT_BLACK, Logger::BG_WHITE, "New connection on", _host, _port);
	int accepted_fd = accept(_socket_fd, NULL, NULL);
	if (accepted_fd == -1) {
		std::stringstream ss;
		ss << "Can't accept new client to " << _host << ":" << _port;
		throw CannotCreateSocketException((_log_msg = ss.str()).data());
	}
	fcntl(accepted_fd, F_SETFL, O_NONBLOCK);
	return new Socket(this, accepted_fd);
}

bool Socket::process_msg(const std::map<std::string, std::string> *mime) {
	char *data[BUF_SIZE];
	size_t not_space_pos;
	
	ssize_t read_status = read(_socket_fd, data, BUF_SIZE);
	if (read_status <= 0)
		throw CannotAccessDataException("Connection is closed");
	_client_msg.append(reinterpret_cast<const char *>(data), read_status);
	not_space_pos = _client_msg.find_first_not_of(" \f\n\r\t\v");
	if (not_space_pos != std::string::npos && not_space_pos != 0)
		_client_msg.erase(0, not_space_pos);
	/* FIXME POTENTIAL BUF IF MSG SIZE IS GREATER THAN 65535 BITES */
	if (not_space_pos != std::string::npos && (_client_msg.find("\r\n\r\n") != std::string::npos
		|| _client_msg.find("\n\n") != std::string::npos)) {
		Logger::print("\nGot request from client ", _socket_fd, ":\t");
		Logger::println(_client_msg);
		Request r(_client_msg.data(), _parent_socket->getServers(), mime);
		_client_msg = r._rep; //FIXME GETTER
		return true;
	} else if (not_space_pos == std::string::npos) {
		_client_msg.clear();
	}
	return false;
}

bool Socket::answer() {
	ssize_t write_status = write(_socket_fd, _client_msg.data(), _client_msg.size());
	if (write_status == -1)
		throw CannotAccessDataException("Can't write data in socket");
	if (write_status == static_cast<ssize_t>(_client_msg.size())) {
		Logger::print("\nMessage to", _socket_fd, ":\t");
		Logger::println(_client_msg);
		Logger::println("");
		_client_msg.clear();
		return true;
	}
	_client_msg.erase(0, static_cast<size_t>(write_status));
	Logger::println(Logger::TXT_BLACK, Logger::BG_YELLOW, "Not all bites were sent to", _socket_fd, 
					"Message is erased and will sent another part again");
	return false;
}

/******************************************************************************************************************
 ************************************************** GETTERS *******************************************************
 *****************************************************************************************************************/

int Socket::getSocketFd() const {
	return _socket_fd;
}

const sockaddr_in &Socket::getAddress() const {
	return _address;
}

int Socket::getPort() const {
	return _port;
}

const std::vector<const Server *> &Socket::getServers() const {
	return _servers;
}

const std::string &Socket::getHost() const {
	return _host;
}

/******************************************************************************************************************
 ************************************************** SETTERS *******************************************************
 *****************************************************************************************************************/

void Socket::setServers(const Server *serv) {
	_servers.push_back(serv);
}



/******************************************************************************************************************
 *********************************************** EXCEPTIONS *******************************************************
 *****************************************************************************************************************/

Socket::CannotCreateSocketException::CannotCreateSocketException(const char *msg): _msg(msg) {}

const char *Socket::CannotCreateSocketException::what() const throw() {
	return _msg;
}

Socket::CannotAccessDataException::CannotAccessDataException(const char *msg): _msg(msg) {}

const char *Socket::CannotAccessDataException::what() const throw() {
	return _msg;
}
