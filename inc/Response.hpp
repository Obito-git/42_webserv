#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.hpp"
#include "Request.hpp"
#include "AutoIndex.hpp"
#include "CGI_Handler.hpp"

class Request;
class Server;
struct Location;
class CGI_Handler;
class AutoIndex;

class Response
{
	private:

		Response();


		Request										*_request;
		const Location								*_location;
		std::string 								_response;
		std::set<std::string>						_index;
		std::string 								_path;
		std::string									_content_type;

	public:

		//CONSTRUCTORS/DESTRUCTORS

		Response(Request *request);
		~Response();

		// GETTERS
		
		const std::string getResponse() const;

		// STATIC FONCTIONS

		static	std::string	_generate_reponse_error(Request *request, int code);
		static	std::string _generate_error_body(const Location *location, short status_code);
		static	std::string _generate_redirection_body(Request *request, short status_code);
		static	std::string	_generate_reponse_headers(Request *request, int code, size_t size);
		static	std::string	_generate_reponse_redirection(Request *request, int code);

	private:
		// GENERATE RESPONSE
		std::string	_concatenate_path();
		void		_path_is_to_folder(std::string path);
		void		_path_is_to_file(std::string path);
		int			_find_content_type(std::string filename);
		std::string	_generate_reponse_ok(int code, std::string code_page);
		std::string	_generate_reponse_cgi(const CGI_Handler &cgi, int status);
		void		_delete_method();
		bool		_is_file(const std::string &path);
};

#endif //WEBSERV_WEBSERV_HPP