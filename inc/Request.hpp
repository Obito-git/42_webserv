#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "parsingRequest.hpp"
#include "Response.hpp"
#include "Webserv_machine.hpp"
#include <cstdlib>


class Webserv_machine;
class Server;
struct Location;
class ClientSocket;

class Request
{
	public:

	//FIXME anton CGI variables
		std::string									_query;
		std::string									_request_body;
		std::string									_path_to_requested_file;
		std::string									_path_info;
		const ClientSocket*							_client_socket;
		
	
	//FIXME CGI variables end
		HTTP_METHOD 								_method;
		std::string									_url;
		std::string									_extention;
		std::string									_http_version;
		std::vector<std::string>					_message;
		std::map<std::string, std::string> 			_header;
		std::string									_host;
		std::string									_content_length;
		std::string									_rep;
		const Server								*_server;
		const Location								*_location;
		const std::vector<const Server*>			_ws;
		const std::map<std::string, std::string> 	*_mime;

	public:
		Request();
		Request(const char *message, const ClientSocket *sock, const std::map<std::string, std::string> *mime);
		Request(const Request &other);
		// Request& operator=(const Request &other) const;
		~Request();


		// GETTERS

		const Location*	getLocation() const;
		const std::string getUrl() const;
		const std::string getExtention() const;


		void setExtention(std::string extention);
		void setPathToFile(std::string path);



		// parsing

		int			_read_message(const char *message);
		void		_read_body(const char * message);
		void		_make_map_of_headers();

		// check request
		int			_check_url(std::vector<std::string> line);
		void		_check_line(int ind);
		int			_check_first_line();
		int			_check_second_line();
		int			_check_body();



		// fill_up_request
		int			_fill_up_request();
		int			_fill_up_content_length();
		void		_fill_up_body();
		void		_fill_up_method(std::string elem);
		void		_fill_up_url(std::string line);
		void		_fill_up_protocol(std::string line);
		void		_fill_up_host(std::map<std::string, std::string>::iterator it);

		// check config
		int			_check_server_name();
		int			_check_location();
		int			_check_methods();

		// reponse
		void		_create_response();

		// printing

		void		_print_message();
		void		_print_dictionary();
		void 		_print_mime(std::map<std::string, std::string> mimes);

};

#endif