#ifndef PARSING_REQUEST_HPP
#define PARSING_REQUEST_HPP

#include "webserv.hpp"

struct message
{
	std::string line;
	message		*next;
};

// enum ERRORS {
// 	ERROR_METHOD = 1
// };


// struct HeaderRequest
// {
// 	std::string			host;
// 	std::string			сonnection;
// 	std::string			cookie;
// 	std::string			user_agent;
// 	std::string			accept_language;
// 	std::string			accept_encoding;
// 	std::string			referer;
// 	std::string			autorization;
// };

// struct Headerresponse
// {
// 	std::string			cache_control;
// 	std::string			content_type;
// 	std::string			content_disposition;
// 	std::string			location_response;
// 	std::string			set_cookie;
// 	std::string			www_authenticate;
// 	std::string			content_encoding;
// 	std::string			server;
// 	std::string			x_powered_by;
// };

// struct Body
// {
// 	/* data */
// };

// struct Url
// {
// 	std::string					protocol;
// 	std::string					subDomen;
// 	std::string					mainDomen;
// 	std::string					domenZone;
// 	short						port;
// 	std::string					adressCatalog;
// 	std::map<std::string, std::string>	getParameter;
// 	std::string					anchor;
// };

#endif
