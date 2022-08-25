#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "parsingRequest.hpp"
#include "Response.hpp"
#include "Webserv_machine.hpp"

class Webserv_machine;
class Server;
struct Location;

class Request
{
	public:

		HTTP_METHOD 						_method;
		std::string							_url;
		std::string							_http_version;
		std::string							_host;
		std::string							_content_length;
		std::map<std::string, std::string> 	_header;
		std::vector<std::string>			_message;
		response							_response;
		std::string							_rep;
		Server								*_server;
		Location							*_location;
		
		/****** WEBSERV MACHINE ********/
		Webserv_machine* _ws;

		// HeaderRequest	_header;
		// Body			_request_body;

	public:
		Request();
		Request(const char *message, Webserv_machine* webserv);
		Request(const Request &other);
		Request& operator=(const Request &other);
		~Request();

		void	_read_message(const char *message);
		void	_check_line(std::string line);
		int		_check_first_line();
		int		_check_second_line();
		void	_make_map_of_headers();

		// REPONSE
		void	_create_response();
		int		_check_server_name();
		int		_check_location();
		int		_check_methods();
		std::string	_concatenate_path();
		std::string	_generate_reponse_ok(std::string code_page);
		std::string	_generate_reponse_error(int code, std::string msg);

		void	_print_message();
		void	_print_dictionary();

		// fill_up_request

		int		_fill_up_request();
		int		_fill_up_content_length();
		void	_fill_up_method(std::string elem);
		void	_fill_up_url(std::string line);
		void	_fill_up_protocol(std::string line);
		void	_fill_up_host(std::map<std::string, std::string>::iterator it);

		static std::string generate_error_body(Location &location, short status_code);
};

#endif