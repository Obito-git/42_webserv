//
// Created by amyroshn on 8/16/22.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <exception>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <ctime>

#include "HttpStatusCode.hpp"

// Colors
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define RESET "\033[0m"


#define MAX_SERV_KEYWORDS	11
#define MAX_LOC_KEYWORDS	9
#define MAX_KEYWORDS 13

std::vector<std::string>* ft_split(std::string s, char delim);
std::string ft_read_file(const std::string& path);

template <class InputIterator>
std::string ft_strjoin(InputIterator begin, InputIterator end, const std::string& delim) {
	std::string tmp;
	while (begin != end)
		tmp.append(std::string(*begin++)).append(delim);
	return tmp;
}

enum HTTP_METHOD {
	GET,
	POST,
	DELETE,
	OTHER,
	INIT
};

enum KEYWORDS {
	LISTEN,
	PORT,
	SERVER_NAME,
	ERR_PAGE,
	CLIENT_BODY_SIZE,
	FILE_UPLOAD,
	METHODS,
	INDEX,
	AUTOINDEX,
	ROOT,
	LOCATION,
	RETURN,
	CGI_PATH
};





#endif //WEBSERV_WEBSERV_HPP
