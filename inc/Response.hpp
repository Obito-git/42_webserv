#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.hpp"
#include "Request.hpp"

class Request;
class Server;
struct Location;

class Response
{
	private:

		Response();


		const Request								*_request;
		const Location								*_location;
		std::string 								_response;
		std::set<std::string>						_index;
		std::string 								_path;
		std::string									_content_type;

	public:

		//CONSTRUCTORS/DESTRUCTORS

		Response(const Request *request);
		~Response();

		// GETTERS
		
		const std::string getResponse() const;

		// STATIC FONCTIONS

		static	std::string	_generate_reponse_error(const Request *request, int code, std::string msg);
		static	std::string _generate_error_body(const Location *location, short status_code);
		static	std::string	_generate_reponse_headers(int code, std::string code_page, size_t size);


	private:
		// GENERATE RESPONSE
		std::string	_concatenate_path();
		void		_path_is_to_folder(std::string path);
		void		_path_is_to_file(std::string path);
		void		_find_content_type(std::string filename);
		std::string	_generate_reponse_ok(int code, std::string code_page);

};

#endif //WEBSERV_WEBSERV_HPP
