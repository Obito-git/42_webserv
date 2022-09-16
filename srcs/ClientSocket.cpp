//
// Created by Anton on 29/08/2022.
//

#include "ClientSocket.hpp"


ClientSocket::ClientSocket(const ListeningSocket *parentSocket, int socket_fd,
						   const sockaddr_in &addr, socklen_t addrLen) : ASocket(socket_fd),
																	_parent_socket(parentSocket),
																	_close_immediately(false), _addr(addr),
																	_addr_len(addrLen) {
	std::stringstream ss;
	ss << "Client " << _socket_fd << " has been connected to ";
	ss << _parent_socket->getHost() << ":" << _parent_socket->getPort();
	Logger::println(Logger::TXT_BLACK, Logger::BG_WHITE, ss.str());
}

bool ClientSocket::check_headers(const Request &req, const std::map<std::string, std::string> *mime) {
	std::map<std::string, std::string>::const_iterator it = req.getHeader().find("Connection");
	if (it != req.getHeader().end() && it->second == "close")
		_close_immediately = true;
	it = req.getHeader().find("Transfer-Encoding");
	if (it != req.getHeader().end()) {
		if (!dechunk(req, mime))
			return false;
	} else if ((it = req.getHeader().find("Content-Length")) != req.getHeader().end()) {
		size_t content_length = std::strtol(it->second.data(), NULL, 10);
		if (req.getRequestBody().length() < content_length) {
			Logger::print(Logger::TXT_BLACK, Logger::BG_YELLOW, "Content-Length is ", content_length,
							" got ");
			Logger::println(Logger::TXT_BLACK, Logger::BG_YELLOW, req.getRequestBody().length(), " bytes");
			return false;
		}
		if (req.getRequestBody().length() > content_length) {
			_request.erase(content_length - 1);
			Request new_req(_request.data(), this, mime);
			_response = new_req.getRep();
		}
	}
	return true;
}

//true if all data recived, in error cases or if not chunked
bool ClientSocket::dechunk(const Request &req, const std::map<std::string, std::string> *mime) {
	std::map<std::string, std::string>::const_iterator te = req.getHeader().find("Transfer-Encoding");
	std::vector<std::string>::iterator it;
	std::string parsed_body;
	size_t content_length = 1;

	if ((te == req.getHeader().end() || te->second != "chunked") || req.getHeader().find("Content-Length") == req.getHeader().end())
		return true;
	if (req.getRequestBody().find("\r\n0\r\n\r\n") == std::string::npos)
		return false;
	std::vector<std::string> parsed = ft_split(req.getRequestBody(), "\r\n");
	for (it = parsed.begin(); it != parsed.end() && content_length; it += 2) {
		errno = 0;
		content_length = std::strtol((*it).data(), NULL, 16);
		if (errno != 0 || (content_length != 0 && (it + 1) == parsed.end()))
			return true;
		if (content_length == 0)
			break;
		if ((it + 1)->length() != content_length)
			return true;
		parsed_body.append(*(it + 1));
	}
	_request = _request.substr(0, _request.find("\r\n\r\n") + 4).append(parsed_body);
	Request new_req(_request.data(), this, mime);
	_response = new_req.getRep();
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
	if (not_space_pos != std::string::npos &&
		(_request.find("\r\n\r\n") != std::string::npos || _request.find("\n\n") != std::string::npos)) {
		Request req(_request, this, mime);
		_response = req.getRep();
		if (check_headers(req, mime)) {
			Logger::print("\nGot request from client ", _socket_fd, ":\t");
			Logger::println(Logger::TXT_BLACK, Logger::BG_CYAN, _request);
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
	if (_response.length() == 0)
		throw std::runtime_error("Empty response, closing connection");
	if (write_status == static_cast<ssize_t>(_response.size()) && _close_immediately)
		throw std::runtime_error("Connection was closed by http \"Connection: close\" header");
	if (write_status == static_cast<ssize_t>(_response.size())) {
		Logger::print("\nMessage to", _socket_fd, ":\t");
		Logger::println(Logger::TXT_BLACK, Logger::BG_CYAN,_response.substr(0, _response.find('\n')));
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

std::string ClientSocket::getClientAddr() const {
	(void) _addr_len;
	return std::string(inet_ntoa(_addr.sin_addr));
}

std::string ClientSocket::getClientPort() const {
	std::stringstream ss;
	ss << ntohs(_addr.sin_port);
	return std::string(ss.str());
}

std::string ClientSocket::getPort() const {
	std::stringstream ss;
	ss << _parent_socket->getPort();
	return std::string(ss.str());
}

const std::vector<const Server *> &ClientSocket::getServers() const {
	return _parent_socket->getServers();
}

std::string ClientSocket::getAddr() const {
	return _parent_socket->getAddr();
}


