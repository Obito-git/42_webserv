//
// Created by Anton on 28/08/2022.
//

#ifndef WEBSERV_CGIHANDLER_HPP
#define WEBSERV_CGIHANDLER_HPP
#include "Webserv_machine.hpp"
#include "Request.hpp"

class Request;


class CGI_Handler {
	
private:
	const Request	*_req;
	char 			**_env;
	int				_env_len;
	std::string 	_cgi_type;
	std::string		_cgi_path;
	int 			_status;
	std::string 	_result;

	void set_environment();
	void launch_cgi();
	bool is_good_type();	
public:
	CGI_Handler(const Request *req);
	int getStatus() const;
	const std::string &getResult() const;

	virtual ~CGI_Handler();
};


#endif //WEBSERV_CGIHANDLER_HPP
