//
// Created by Anton on 19/08/2022.
//

#ifndef WEBSERV_CONFIGPARSER_HPP
#define WEBSERV_CONFIGPARSER_HPP
#include "webserv.hpp"
#include "Server.hpp"


class ConfigParser {
private:
	typedef std::string::iterator str_iter;
	std::string			    	_error_msg;
	std::string 				_path;
	std::vector<Server *>	    _servers;
public:
	//constructor
	ConfigParser(const std::string &path);
	//getter
	const std::vector<Server *> &getServers();

private:
	/******************************************************************************************************************
	 ************************************************** PARSING *******************************************************
	 *****************************************************************************************************************/
	/* parsing file methods */
	void parse_config(const char *path);
	static str_iter skip_comments_and_spaces(std::string& file_content, str_iter it, int& line);
	std::string& find_unexpected_token(std::string& s, const char* token, int line);
	int find_server_keyword(std::string& file_content, str_iter& it, int& line);
	void parse_server_block(std::string& file_content, str_iter& it, int& line);

	/* parsing args of parameters methods*/
	void parse_listen_args(Server* s, std::vector<std::string>& args, int line, std::string& file_content);
	void parse_port_args(Server* s, std::vector<std::string>& args, int line, std::string& file_content);
	void parse_servername_args(Server* s, std::vector<std::string>& args, int line, std::string& file_content);
	void parse_errorpages_args(Location& loc, std::vector<std::string>& args, int line, std::string& file_content);
	void parse_bodysize_args(Location& loc, std::vector<std::string>& args, int line, std::string& file_content);
	void parse_fileupload_args(Location& loc, std::vector<std::string>& args, int line, std::string& file_content);
	void parse_methods_args(Location& loc, std::vector<std::string>& args, int line, std::string& file_content);
	void parse_index_args(Location& loc, std::vector<std::string>& args, int line, std::string& file_content);
	void parse_autoindex_args(Location& loc, std::vector<std::string>& args, int line, std::string& file_content);
	//void parse_return_args(Server* s, std::vector<std::string>& args, int line, std::string& file_content);
	
	
/******************************************************************************************************************
 ************************************************ EXCEPTIONS *******************************************************
 *****************************************************************************************************************/

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
