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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <exception>

#include <fstream>
#include <sstream>
#include <ctime>


enum HTTP_METHOD {
	GET = 0,
	POST = 1,
	DELETE = 2,
	OTHER = 3,
	INIT = 4
};

#endif //WEBSERV_WEBSERV_HPP
