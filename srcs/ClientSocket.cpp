//
// Created by Anton on 29/08/2022.
//

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(const ListeningSocket *parentSocket, int socket_fd) : ASocket(socket_fd),
					_parent_socket(parentSocket), _close_immediately(false) {
	std::stringstream ss;
	ss << "Client " << _socket_fd << " has been connected to ";
	ss << _parent_socket->getHost() << ":" << _parent_socket->getPort();
	Logger::println(Logger::TXT_BLACK, Logger::BG_WHITE, ss.str());
}

/* FIXME POTENTIAL BUg IF MSG SIZE IS GREATER THAN 65535 BITES */
bool ClientSocket::check_headers(const std::map<std::string, std::string> &headers) {
	std::map<std::string, std::string>::const_iterator it = headers.find("Content-Length");
	//FIXME check length of body here
	it = headers.find("Connection");
	if (it != headers.end() && it->second == "close")
		_close_immediately = true;
	return true;
}

bool ClientSocket::recv_msg(const std::map<std::string, std::string> *mime){
	char *data[BUF_SIZE];
	size_t not_space_pos;

	ssize_t read_status = read(_socket_fd, data, BUF_SIZE);
	if (read_status <= 0) //Can't read or got empty msg
		throw CannotAccessDataException("Connection is closed");
	_request.append(reinterpret_cast<const char *>(data), read_status);
	not_space_pos = _request.find_first_not_of(" \f\n\r\t\v");
	//if (not_space_pos != std::string::npos && not_space_pos != 0) FIXME don't need
	//	_request.erase(0, not_space_pos);
	if (not_space_pos != std::string::npos &&
		(_request.find("\r\n\r\n") != std::string::npos || _request.find("\n\n") != std::string::npos)) {
		Request req(_request.data(), _parent_socket->getServers(), mime);
		if (check_headers(req._header)) {
			_response = req._rep;
			Logger::print("\nGot request from client ", _socket_fd, ":\t");
			Logger::println(Logger::TXT_BLACK, Logger::BG_CYAN, _request.substr(0, _request.find('\n')));
			_request.clear();
			return true;
		}
	} else if (not_space_pos == std::string::npos)
		_request.clear();
	return false; //got only newline characters
}

bool ClientSocket::answer() {
	ssize_t write_status = write(_socket_fd, _response.data(), _response.size());
	if (write_status == -1)
		throw CannotAccessDataException("Can't write data in socket");
	if (write_status == static_cast<ssize_t>(_response.size()) && _close_immediately)
		throw std::runtime_error("Connection was closed by http \"Connection: close\" header");
	if (write_status == static_cast<ssize_t>(_response.size())) {
		Logger::print("\nMessage to", _socket_fd, ":\t");
		Logger::println(Logger::TXT_BLACK, Logger::BG_CYAN,_response.substr(0, _response.find('\n')));
		//Logger::println(_client_msg);
		Logger::println("");
		_response.clear();
		return true;
	}
	_response.erase(0, static_cast<size_t>(write_status));
	Logger::println(Logger::TXT_BLACK, Logger::BG_YELLOW, "Not all bites were sent to", _socket_fd,
					"Message is erased and will sent another part again");
	return false;
}

void ClientSocket::close() {
	if (_socket_fd != -1) {
		Logger::print(Logger::TXT_BLACK, Logger::BG_WHITE, "Client", _socket_fd, "has been disconnected from ");
		Logger::println(Logger::TXT_BLACK, Logger::BG_WHITE, 
						_parent_socket->getHost() ,_parent_socket->getPort());
		::close(_socket_fd);
		_socket_fd = -1;
	}
}

ClientSocket::~ClientSocket() {
	close();
}
