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
#include <istream>
#include <iterator>
#include <sstream>
#include <algorithm>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <exception>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <cstring>
#include <cerrno>

#include "HttpStatusCode.hpp"
#include "Logger.hpp"

#define MAX_SERV_KEYWORDS	11
#define MAX_LOC_KEYWORDS	9
#define MAX_KEYWORDS 13
#define MAX_CGI_TYPES 3
#define BUF_SIZE 65535
#define MIME_FILE "dependencies/mime"

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


std::vector<std::string>	ft_split(std::string s, char delim);
std::vector<std::string>	ft_split(std::string s, const std::string &delim);
std::string					ft_read_file(const std::string& path);
std::string					get_method_name(HTTP_METHOD m);
std::string					ft_to_upper_case(const std::string &s);
bool						ft_isnumeric(const char *s);

template <class InputIterator>
std::string ft_strjoin(InputIterator begin, InputIterator end, const std::string& delim) {
	std::string tmp;
	while (begin != end) {
		tmp.append(std::string(*begin));
		if (++begin != end)
			tmp.append(delim);
	}
	return tmp;
}

#endif //WEBSERV_WEBSERV_HPP
