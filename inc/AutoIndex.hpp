#ifndef AUTO_INDEX_HPP
#define AUTO_INDEX_HPP

#include "webserv.hpp"
#include "Request.hpp"
#include "CGI_Handler.hpp"

class Request;
class Server;
struct Location;
class CGI_Handler;

class AutoIndex
{
	private:

		// AutoIndex();

	public:

		//CONSTRUCTORS/DESTRUCTORS

		AutoIndex();
		virtual ~AutoIndex();

		// STATIC FONCTIONS

		static	std::string	generate_autoindex_page(std::string path, Request* req);

	private:

		static  std::string _getLink(std::string const &dirEntry, std::string dirName, Request* req);
};

#endif //WEBSERV_WEBSERV_HPP