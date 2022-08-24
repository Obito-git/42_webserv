#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.hpp"

struct response
{
	std::string		_http_version;
	int				_status_code;
	std::string		_status_message;
	std::string		_date;
	std::string		_server;
	std::string		_content_type;
	size_t			_content_length;
	// std::map<int, std::string>	_status;

};

#endif //WEBSERV_WEBSERV_HPP
