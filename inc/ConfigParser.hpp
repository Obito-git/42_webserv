//
// Created by Anton on 19/08/2022.
//

#ifndef WEBSERV_CONFIGPARSER_HPP
#define WEBSERV_CONFIGPARSER_HPP
#include "webserv.hpp"
#include "Server.hpp"
#include "CGI_Handler.hpp"

class Server;
struct Location;

class ConfigParser {
private:
	typedef std::string::iterator str_iter;
	
	int									_line_number;
	std::string					    	_error_msg;
	std::string 						_path;
	std::vector<Server *>	    		_servers;
	std::string 						_webserv_location;
public:
	//constructor
	ConfigParser(const std::string &path, const std::string &webserv_location);
	//getter
	const std::vector<Server *> &getParsedServers();
	std::vector<Server *> &getServers();

	static std::map<std::string, std::string> *getMime() ;


private:
	/******************************************************************************************************************
	 ************************************************** PARSING *******************************************************
	 *****************************************************************************************************************/
	/* parsing file methods */
	void parse_config(const char *path);
	str_iter skip_comments_and_spaces(std::string& file_content, str_iter it);
	std::string& find_unexpected_token(std::string s, const char* token);
	int find_server_keyword(std::string& file_content, str_iter& it);
	void parse_server_block(std::string& file_content, str_iter& it);
	std::vector<std::string> parse_parameter_args(std::string &file_content, str_iter &it);
	std::pair<int, std::string> skip_location_block(std::string &file_content, str_iter &it);
	void parse_locations(Server *s, const Location &def, std::vector<std::pair<int, std::string> > &loc);	
	

	/* parsing args of parameters methods*/
	void parse_listen_args(Server* s, std::vector<std::string>& args);
	void parse_port_args(Server* s, std::vector<std::string>& args);
	void parse_servername_args(Server* s, std::vector<std::string>& args);
	void parse_cgi_path(Server *s, std::vector<std::string>& args);
	void parse_errorpages_args(Location& loc, std::vector<std::string>& args);
	void parse_bodysize_args(Location& loc, std::vector<std::string>& args);
	void parse_fileupload_args(Location& loc, std::vector<std::string>& args);
	void parse_methods_args(Location& loc, std::vector<std::string>& args);
	void parse_index_args(Location& loc, std::vector<std::string>& args);
	void parse_autoindex_args(Location& loc, std::vector<std::string>& args);
	void parse_root_args(Location& loc, std::vector<std::string>& args);
	void parse_return_args(Location &loc, std::vector<std::string>& args);
	
	void check_server_pages(Server *s);
	void is_good_error_page(Server* s, Location& loc, short code, const std::string& path);
	void is_correct_servernames();
	
	//void parse_return_args(Server* s, std::vector<std::string>& args, std::string& file_content);
	
/******************************************************************************************************************
 ************************************************ EXCEPTIONS *******************************************************
 *****************************************************************************************************************/

public:
	class ConfigUnexpectedToken: public std::exception {
	private:
		const char *msg;
	public:
		ConfigUnexpectedToken(const char *m);
		virtual const char *what() const throw();
	};

	class ConfigCriticalError: public std::exception {
	private:
		const char *msg;
	public:
		ConfigCriticalError(const char *m);
		virtual const char *what() const throw();
	};

	class ConfigNoRequiredKeywords: public std::exception {
	public:
		virtual const char *what() const throw();
	};


	
};


#endif //WEBSERV_CONFIGPARSER_HPP
