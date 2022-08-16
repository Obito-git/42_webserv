//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_LOCATION_HPP
#define WEBSERV_LOCATION_HPP
#include "webserv.hpp"

struct Location {
private:
	std::string _location;
	std::string _root;
	bool _autoindex;
	std::vector<std::string> _index;
	bool _file_upload;
	unsigned long _max_body_size;
	std::string ret_value;
	std::string _cgi_path;
	std::vector<HTTP_METHOD> _forbidden_methods;
};


#endif //WEBSERV_LOCATION_HPP
